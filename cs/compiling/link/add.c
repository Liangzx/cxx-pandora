int g_sum = 0;

int add(int a, int b) {
  g_sum = a + b;
  return g_sum;
}

//  gcc -fpic -shared -g -o libadd_debug.so add.c