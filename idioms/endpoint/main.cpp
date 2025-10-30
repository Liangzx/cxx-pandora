#include "endpoint.h"

#include <iostream>

using Logger = std::string;

endpoint::Middleware LoggingMiddleware(Logger logger) {
  return [logger](endpoint::Endpoint next) -> endpoint::Endpoint {
    return [logger, next](endpoint::Context ctx,
                          endpoint::Request req) -> endpoint::Response {
      std::cout << logger << ": " << req << std::endl;
      return next(ctx, req);
    };
  };
}

int main() {
  auto my_handler = [](endpoint::Context ctx, endpoint::Request req) {
    return endpoint::Response{};
  };

  auto logging_my_handler = LoggingMiddleware("my_handler");

  logging_my_handler(my_handler)(endpoint::Context{}, "hello");

  return 0;
}
