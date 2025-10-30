// endpoint.hpp
#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

// 仅使用标准库类型作为请求/响应的“字节视图”
using Request = std::string_view;
using Response = std::string_view;

// 统一结果：成功返回响应，失败返回错误信息
using Result = std::variant<Response, std::string>;

// 取消令牌：支持请求级取消
struct CancellationToken {
  std::shared_ptr<bool> cancelled{std::make_shared<bool>(false)};
  void cancel() { *cancelled = true; }
  bool is_cancelled() const { return *cancelled; }
};

// 上下文：携带取消与截止时间（可扩展 deadline、baggage 等）
struct Context {
  CancellationToken token;
  using TimePoint = std::chrono::steady_clock::time_point;
  TimePoint deadline{std::chrono::steady_clock::time_point::max()};
};

// Endpoint：同步与异步两种签名
using EndpointSync = std::function<Result(Context, Request)>;
using EndpointAsync =
    std::function<void(Context, Request, std::function<void(Result)>)>;

// 编解码器概念：把字节视图序列化/反序列化为具体类型（此处仅演示 string_view）
struct LengthPrefixedCodec {
  // 把 T 序列化为字节流（示例：string_view -> 含长度头的字节）
  template <typename T> static std::vector<std::byte> serialize(T const &v) {
    std::string s = std::to_string(v.size()) + "\n" + std::string(v);
    return std::vector<std::byte>(
        reinterpret_cast<std::byte const *>(s.data()),
        reinterpret_cast<std::byte const *>(s.data() + s.size()));
  }

  // 从字节流反序列化为 T（示例：解析长度头，再读对应字节数）
  template <typename T> static Result deserialize(std::string_view buf) {
    auto pos = buf.find('\n');
    if (pos == std::string_view::npos) {
      return std::string{"missing length header"};
    }

    auto n = 0ULL;
    try {
      n = std::stoull(std::string(buf.substr(0, pos)));
    } catch (...) {
      return std::string{"invalid length"};
    }

    if (pos + 1 + n > buf.size()) {
      return std::string{"incomplete body"};
    }

    return Response{buf.substr(pos + 1, n)};
  }
};

// 传输抽象：发送/接收字节流
struct Transport {
  virtual ~Transport() = default;
  virtual void start_read(std::function<void(std::vector<std::byte>)>) = 0;
  virtual void write(std::vector<std::byte>,
                     std::function<void(boost::system::error_code)>) = 0;
  virtual void close() = 0;
};
