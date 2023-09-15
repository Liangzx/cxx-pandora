int g_sub = 0;

int sub(int a, int b) {
  g_sub = a - b;
  return g_sub;
}

// gcc -fpic -shared -g -o libsub_debug.so sub.c