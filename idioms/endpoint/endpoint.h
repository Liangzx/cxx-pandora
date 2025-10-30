#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <string>

namespace endpoint {

// 仅使用标准库类型作为请求/响应的“字节视图”
using Request = std::string;
using Response = std::string;
using Error = std::string;

// // 统一结果：成功返回响应，失败返回错误信息
// using Result = std::variant<Response, Error>;

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
  TimePoint deadline{TimePoint::max()};
};

using Endpoint = std::function<Response(Context, Request)>;

using Middleware = std::function<Endpoint(Endpoint)>;

} // namespace endpoint
