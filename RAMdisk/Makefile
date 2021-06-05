obj-m := memory.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	sudo insmod memory.ko

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	sudo rmmod memory.ko
	sudo rm /dev/memory

update: clean all

prepare:
	sudo mknod /dev/memory c 60 0
	sudo chmod 666 /dev/memory

client:
	gcc client.c -o client.o
