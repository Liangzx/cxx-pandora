// 异步 Endpoint 与 TCP 传输示例​

// async_endpoint.cpp
#include "endpoint.h"
#include <boost/asio.hpp>
#include <iostream>
#include <memory>

using boost::asio::ip::tcp;

// 异步 Echo：把请求体原样回写（长度前缀编码）
void echo_async(Context ctx, Request req, std::function<void(Result)> cb) {
  if (ctx.token.is_cancelled()) {
    return cb(std::string{"cancelled"});
  }

  if (std::chrono::steady_clock::now() > ctx.deadline) {
    return cb(std::string{"deadline exceeded"});
  }

  // 直接把请求体当响应体回写（长度前缀）
  std::string body(req);
  auto encoded = LengthPrefixedCodec::serialize(body);
  cb(Response{encoded.data(), encoded.size()});
}

// 最小 TCP 传输：把 EndpointAsync 包装成可读写的连接
struct TcpTransport : Transport, std::enable_shared_from_this<TcpTransport> {
  tcp::socket sock_;
  std::function<void(std::vector<std::byte>)> on_read_;
  std::function<void(boost::system::error_code)> on_write_;
  std::vector<std::byte> read_buf_;
  static constexpr size_t max_read = 64 * 1024;

  explicit TcpTransport(tcp::socket s) : sock_(std::move(s)) {}

  void
  start_read(std::function<void(std::vector<std::byte>)> on_read) override {
    on_read_ = std::move(on_read);
    do_read();
  }

  void write(std::vector<std::byte> data,
             std::function<void(boost::system::error_code)> on_write) override {
    on_write_ = std::move(on_write);
    auto self = shared_from_this();
    boost::asio::async_write(sock_, boost::asio::buffer(data),
                             [self](boost::system::error_code ec, std::size_t) {
                               self->on_write_(ec);
                             });
  }

  void close() override {
    boost::system::error_code ec;
    sock_.close(ec);
  }

private:
  void do_read() {
    auto self = shared_from_this();
    read_buf_.resize(max_read);
    sock_.async_read_some(boost::asio::buffer(read_buf_),
                          [self](boost::system::error_code ec, std::size_t n) {
                            if (!ec) {
                              read_buf_.resize(n);
                              self->on_read_(std::move(read_buf_));
                            } else {
                              self->on_read_({});
                            }
                          });
  }
};

// 把任意 EndpointAsync 适配到 Transport（处理编解码、生命周期、回写）
template <typename Codec>
struct EndpointToTransportAdapter
    : std::enable_shared_from_this<EndpointToTransportAdapter<Codec>> {
  Transport &transport_;
  EndpointAsync ep_;
  Codec codec_;

  explicit EndpointToTransportAdapter(Transport &t, EndpointAsync e,
                                      Codec c = {})
      : transport_(t), ep_(std::move(e)), codec_(std::move(c)) {}

  void start() {
    auto self = shared_from_this();
    transport_.start_read([self](std::vector<std::byte> bytes) {
      if (bytes.empty()) {
        transport_.close();
        return;
      }
      std::string_view body(reinterpret_cast<char const *>(bytes.data()),
                            bytes.size());
      ep_(Context{}, body, [self](Result r) {
        if (std::holds_alternative<Response>(r)) {
          auto const &resp = std::get<Response>(r);
          auto encoded = codec_.serialize(resp);
          self->transport_.write(encoded, [](boost::system::error_code) {});
        }
        // 忽略错误结果，仅做回写；生产可加日志/关闭连接
        self->start_read([self](std::vector<std::byte> b) { self->start(); });
      });
    });
  }
};
