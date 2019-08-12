CXX = i686-elf-g++
CC = i686-elf-gcc
AS = nasm
CXXFLAGS = -Wall -Wextra -O2 -pedantic -ffreestanding -fno-exceptions -fno-rtti
CCFLAGS = -Wall -Wextra -O2 -pedantic -ffreestanding
OPTS = -MD -MP -Iinclude -Iinclude/kstd

CRTI_OBJ = obj/crti.o
CRTBEGIN_OBJ := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtend.o)
CRTN_OBJ = obj/crtn.o

FILTER_OUT := src/crti.s src/crtn.s
SRC := $(filter-out $(FILTER_OUT), $(wildcard src/*.cpp src/*.s))

TMP_OBJ := $(patsubst src/%, obj/%, $(SRC:.s=.o))
OBJ := obj/main.o $(filter-out obj/main.o,$(patsubst src/%, obj/%, $(TMP_OBJ:.cpp=.o)))

DEP := $(OBJ:%.o=%.d)

.PHONY: all clean

all: main iso qemu

main: $(CRTI_OBJ) $(CRTBEGIN_OBJ) obj/boot.o $(OBJ) obj/tinyprintf.o $(CRTEND_OBJ) $(CRTN_OBJ)
	$(CXX) -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib $^ -lgcc

iso:
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub2-mkrescue -quiet -o myos.iso isodir

qemu:
	qemu-system-i386 -cdrom myos.iso

obj/%.o: src/%.s
	$(AS) -felf32 $< -o $@

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(OPTS) -c $< -o $@

obj/tinyprintf.o: src/tinyprintf.c
	$(CC) $(CCFLAGS) $(OPTS) -c $< -o $@

clean:
	rm -rf obj/* main myos* isodir/

-include $(DEP)
