#set kernel build directory
KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)


# Targets
KMOD := ram_monitor.ko
USER_BIN := cpu_monitor

obj-m += ram_monitor.o

all: $(KMOD) $(USER_BIN)

$(KMOD): ram_monitor.c
	$(MAKE) -C $(KDIR) M=$(PWD) modules

$(USER_BIN): cpu_monitor.c
	$(CROSS_COMPILE)gcc -Wall -Wextra -o $(USER_BIN) cpu_monitor.c

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f $(USER_BIN)
