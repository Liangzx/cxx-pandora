template <typename T> void f(T = "") {}

int main() {
  f(1);
  f();
  return 0;
}
