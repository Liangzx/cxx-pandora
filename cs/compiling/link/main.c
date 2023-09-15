extern int add(int a, int b);
extern int sub(int a, int b);

int main() {
  add(0xb, 0xc);
  add(0xb, 0xc);
  sub(0xf, 0xd);
  sub(0xf, 0xd);
  return 0;
}
// gcc -o main main.c ./libadd_debug.so ./libsub_debug.so -g