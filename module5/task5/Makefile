obj-m += nlKernel.o

all: user
    gcc user.c -o user
    make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
    rm -f user
    make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean