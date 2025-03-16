obj-m := pcd_platform_driver.o

KERNEL_DIR ?= "${STAGING_KERNEL_DIR}"
KERNEL_SRC ?= $(KERNEL_DIR)

# Path to the directory that contains the source file(s) to compile
PWD := $(shell pwd) 

default:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(PWD) clean
	