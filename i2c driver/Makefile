DIR = /home/david/linux/MX6ULL/Linux/linux-imx-rel_imx_4.1.15_2.1.0_ga

currentpath := $(shell pwd)

obj-m := iic.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(DIR) M=$(currentpath) modules
clean:
	$(MAKE) -C $(DIR) M=$(currentpath) clean