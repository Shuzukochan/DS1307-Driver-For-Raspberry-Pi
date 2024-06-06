obj-m += ds1307.o

all: module dt gcc

module:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
dt: ds1307_module.dts
	dtc -@ -I dts -O dtb -o ds1307_module.dtbo ds1307_module.dts
gcc:
	gcc -c ds1307_library.c -o  ds1307_library.o
	ar rc libds1307_library.a ds1307_library.o
	gcc ds1307_userspace_example.c -L. -lds1307_library -o ds1307_userspace_example
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	rm -rf ds1307_module.dtbo ds1307_userspace_example
	
