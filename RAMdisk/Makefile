obj-m := memory.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	sudo insmod memory.ko

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	sudo rmmod memory.ko

update: clean install
