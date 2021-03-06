ifneq ($(KERNELRELEASE),)
	obj-m:=chedv.o
else
	KERNELDIR?=/lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf*~ *.o *.ko *.mod.c . *.cmd modules.order Module.markers Module.symvers .tmp_versions
endif
