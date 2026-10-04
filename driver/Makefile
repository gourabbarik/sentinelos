kobj-m += sentinel_events.o

KERNEL_DIR ?= $(HOME)/wsl-kernel

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
