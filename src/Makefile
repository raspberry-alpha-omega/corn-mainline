#ARCH = /drives/c/devtools/yagarto-20121222/bin/arm-none-eabi-
export CC = ${ARCH}gcc
export CPP = ${ARCH}g++
export AS = ${ARCH}as
export LD = ${ARCH}ld
export AR = ${ARCH}ar
export OBJCOPY = ${ARCH}objcopy
export PLATFORM = raspi

# Release Version -> Optimize 
#CFLAGS = -O3 -std=gnu99 -Werror -D__$(PLATFORM)__ -DRASPBERRY_PI
#ASFLAGS =

export CFLAGS = -O0 -g -std=gnu99 -Werror -D__$(PLATFORM)__ -DRASPBERRY_PI -fno-builtin 
export ASFLAGS = -g

CFLAGS_FOR_TARGET = -mcpu=arm1176jzf-s
ASFLAGS_FOR_TARGET = -mcpu=arm1176jzf-s
LDFLAGS = -nostdlib -static --error-unresolved-symbols 

MODULES := generic raspi
SRC_DIR := $(addprefix ./,$(MODULES))
INC_DIR := $(addsuffix /include,$(SRC_DIR))
BUILD_DIR := $(addsuffix /build,$(SRC_DIR))

ASRC     := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.s))
AOBJ     := $(ASRC:.s=.o)
CSRC     := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
COBJ     := $(CSRC:.c=.o)

INCLUDES := -I. $(addprefix -I,$(SRC_DIR) $(INC_DIR))

vpath %.c $(SRC_DIR)
vpath %.cpp $(SRC_DIR)
vpath %.s $(SRC_DIR)

%.o: %.c
	$(CC) $(CFLAGS_FOR_TARGET) $(INCLUDES) $(CFLAGS) -c -o $*.o $<

%.o: %.s
	$(AS) $(ASFLAGS_FOR_TARGET) $(INCLUDES) $(ASFLAGS) -o $*.o $<

OBJ = $(AOBJ) $(COBJ)

../bin/kernel.img: ../bin/kernel.elf
	${OBJCOPY} -O binary $< $@

../bin/kernel.elf: ../raspi.ld $(OBJ) 
	${LD} ${LDFLAGS}  $(OBJ) -Map ../bin/kernel.map -o $@ -T ../raspi.ld

clean:
	rm -f ../bin/*.elf bin/*.img ../bin/*.map $(OBJ)
