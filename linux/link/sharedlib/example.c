// example.c
int global_var = 0;

void increment_global_var() { global_var++; }

int get_global_var() { return global_var; }

int* get_global_var_ptr() { return &global_var; }

// gcc -fpic -shared -o libexample.so example.c
