export FLAVOURS = host raspi
export MODULES = generic raspi stubs test

default: all

CFLAGS_GLOBAL = -O0 -g -std=gnu99 -Werror 
CFLAGS_TARGET = -D__$(PLATFORM)__ -DRASPBERRY_PI -fno-builtin -mcpu=arm1176jzf-s
CFLAGS_HOST = 

LDFLAGS_GLOBAL = --error-unresolved-symbols 
LDFLAGS_TARGET = -static -nostdlib 
LDFLAGS_HOST = 


all: host target

target: export ARCH = /drives/c/devtools/yagarto-20121222/bin/arm-none-eabi-
target: export PLATFORM = raspi
target: export CFLAGS = $(CFLAGS_GLOBAL) $(CFLAGS_TARGET) 
target: export LDFLAGS = $(LDFLAGS_GLOBAL) $(LSFLAGS_TARGET) 
target: export ASFLAGS = -mcpu=arm1176jzf-s -g

host: export ARCH = # /drives/c/devtools/MinGW/bin/
host: export PLATFORM = host
host: export CFLAGS = $(CFLAGS_GLOBAL) $(CFLAGS_HOST) 
host: export LDFLAGS = $(LDFLAGS_GLOBAL) $(LSFLAGS_HOST) 

host: FORCE
	cd src/stubs; $(MAKE)
	cd src/generic; $(MAKE)
	cd src/test; $(MAKE)
	${ARCH}gcc -o bin/test ${LDFLAGS} lib/host/test.a lib/host/generic.a lib/host/stubs.a
	bin/test

target: FORCE
	cd src/raspi; $(MAKE)
	cd src/generic; $(MAKE)
	${ARCH}ld ${LDFLAGS} lib/raspi/generic.a lib/raspi/raspi.a lib/raspi/generic.a lib/raspi/raspi.a -Map bin/kernel.map -o bin/kernel.elf -T raspi.ld
	${ARCH}objcopy -O binary bin/kernel.elf bin/kernel.img
	
clean: FORCE
	rm -f bin/kernel.*
	rm -f $(foreach flav,$(FLAVOURS),lib/$(flav)/*.a)
	rm -f $(foreach mod,$(MODULES),$(foreach flav,$(FLAVOURS),src/$(mod)/$(flav)/*.o))
	rm -f $(foreach mod,$(MODULES),src/$(mod)/*.o)
	
FORCE:
