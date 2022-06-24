CONFIG_MODULE_SIG=n
TARGET		:= ThunderSoft_Project
OBJS		:= ./src/core_thread.o ./src/core_timer.o ./src/core_main.o ./src/core_semaphore.o ./src/core_char.o 

ifneq ($(KERNELRELEASE),)
	obj-m:= $(TARGET).o
	$(TARGET)-objs += $(OBJS)
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	@rm -rf *~ *.o *.ko *.mod.c *.mod .*.cmd modules.order Module.makers Module.symvers .tmp_versions *dwo ./src/*.dwo ./src/*.o ./src/.*.o.cmd

endif

