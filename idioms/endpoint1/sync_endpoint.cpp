// 同步 Endpoint 与编解码器示例​
// sync_endpoint.cpp
#include "endpoint.h"
#include <iostream>

// 示例业务：回声，并在路径中携带 /echo/<msg>
Result echo_sync(Context ctx, Request req) {
  if (ctx.token.is_cancelled()) {
    return std::string{"cancelled"};
  }

  if (std::chrono::steady_clock::now() > ctx.deadline) {
    return std::string{"deadline exceeded"};
  }

  std::string body(req);
  if (body.rfind("/echo/", 0) == 0) {
    return Response{body.data() + 6, body.size() - 6}; // 去掉 "/echo/"
  }

  return std::string{"unknown path"};
}
