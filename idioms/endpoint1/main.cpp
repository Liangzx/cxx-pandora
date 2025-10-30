// main.cpp
#include "endpoint.h"
#include <boost/asio.hpp>
#include <iostream>
#include <thread>

using boost::asio::ip::tcp;

int main() {
  boost::asio::io_context ioc;

  // 1) 同步 Endpoint 演示
  {
    std::cout << "[sync] ";
    Result r = echo_sync(Context{}, "hello sync");
    if (std::holds_alternative<Response>(r)) {
      std::cout << std::string(std::get<Response>(r)) << "\n";
    } else {
      std::cout << "ERROR: " << std::get<std::string>(r) << "\n";
    }

    // 超时演示
    Context ctx;
    ctx.deadline =
        std::chrono::steady_clock::now() + std::chrono::milliseconds(1);
    r = echo_sync(ctx, "too slow");
    if (std::holds_alternative<Response>(r)) {
      std::cout << std::string(std::get<Response>(r)) << "\n";
    } else {
      std::cout << "timeout: " << std::get<std::string>(r) << "\n";
    }
  }

  // 2) 异步 Endpoint + TCP 演示
  {
    tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 18081));
    std::cout << "[async] listening on 127.0.0.1:18081\n";

    auto ep = [](Context ctx, Request req, std::function<void(Result)> cb) {
      if (ctx.token.is_cancelled())
        return cb(std::string{"cancelled"});
      if (std::chrono::steady_clock::now() > ctx.deadline)
        return cb(std::string{"deadline exceeded"});
      std::string body(req);
      if (body == "error")
        return cb(std::string{"simulated error"});
      cb(Response{body.data(), body.size()});
    };

    std::thread t([&ioc] { ioc.run(); });

    acceptor.async_accept([&](boost::system::error_code ec, tcp::socket sock) {
      if (ec) {
        std::cerr << "accept: " << ec.message() << "\n";
        return;
      }
      auto trans = std::make_shared<TcpTransport>(std::move(sock));
      auto adapter =
          std::make_shared<EndpointToTransportAdapter<LengthPrefixedCodec>>(
              *trans, ep, LengthPrefixedCodec{});
      adapter->start();
    });

    // 客户端：发 /echo/hello 并等待回写
    std::thread client([&ioc]() {
      tcp::socket s(ioc);
      boost::asio::connect(s, tcp::resolver(ioc).resolve("127.0.0.1", "18081"));

      std::string msg = "/echo/hello async";
      auto encoded = LengthPrefixedCodec::serialize(msg);
      boost::asio::write(s, boost::asio::buffer(encoded));

      std::vector<std::byte> reply(1024);
      size_t n = s.read_some(boost::asio::buffer(reply));
      reply.resize(n);

      std::string_view body(reinterpret_cast<char const *>(reply.data()),
                            reply.size());
      auto pos = body.find('\n');
      if (pos != std::string_view::npos &&
          pos + 1 + std::stoull(std::string(body.substr(0, pos))) ==
              body.size()) {
        body.remove_prefix(pos + 1);
        std::cout << "[async] received: " << body << "\n";
      } else {
        std::cout << "[async] decode failed\n";
      }
      s.close();
      ioc.stop();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 等待连接建立
    t.join();
    client.join();
  }

  return 0;
}
