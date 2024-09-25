#include <functional>
#include <iostream>
#include <string>

struct Request
{
  std::string method;
  std::string path;
};

struct Response
{
  int status = 0;
  std::string body;
};

using Handler = std::function<Response(Request)>;

using Middleware = std::function<Handler(Handler)>;

Handler loggingMiddleware(Handler next)
{
  return [next](Request req) -> Response {
    std::cout << "logging...." << req.path << std::endl;
    return next(req);
  };
}

Handler authMiddleware(Handler next)
{
    return [next](Request req) -> Response {
    std::cout << "auth...." << req.path << std::endl;
    return next(req);
  };
}

void func(Middleware middleware)
{

}

int main()
{
  auto my_handler = [](Request req) {
    return Response{1, "hello"};
  };

  Request req{"get", "/admin"};
  Middleware m = loggingMiddleware;
  auto handler = loggingMiddleware(authMiddleware(my_handler));
  handler(req);

  return 0;
}
