all: v1/libadd.so v2/libadd.a libmylib.so main

v1/libadd.so: v1/add.c
	gcc -shared -fPIC -o $@ $<

# v2/libadd.so: v2/add.c
# 	gcc -shared -fPIC -o $@ $<

v2/libadd.a: v2/add.c | v2
	gcc -c $< -o $(@D)/add.o
	ar rcs $@ $(@D)/add.o

libmylib.so: mylib.c
	gcc -shared -fPIC -o $@ $< -L./v1 -ladd

main: main.c
	gcc main.c -o $@ -L./v2 -ladd -ldl

clean:
	rm -f *.so v1/*.so v2/*.so main

test: all
	export LD_LIBRARY_PATH=./v1:./v2:.; ./main
