ARCH=x86
CROSS_COMPILE=

# KVERSION = $(shell uname -r)
# KERN_DIR =  /lib/modules/$(KVERSION)/build
KERN_DIR=/lib/modules/5.15.0-60-generic/build
CURRENT_PATH = $(shell pwd)

all:
	make -C $(KERN_DIR) M=${CURRENT_PATH} modules

clean:
	make -C $(KERN_DIR) M=${CURRENT_PATH} modules clean
	rm -rf modules.order

obj-m += helloworld.o
