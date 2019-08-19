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

FILTER_OUT_NASM := obj/crti.o obj/crtn.o
NASM_SRC := $(shell find src/ -wholename *.nasm)
NASM_OBJ := $(filter-out $(FILTER_OUT_NASM), $(patsubst src/%, obj/%, $(NASM_SRC:.nasm=.o)))

FILTER_OUT_CPP := obj/main.o obj/Terminal.o
CPP_SRC := $(shell find src/ -wholename *.cpp)
CPP_OBJ := $(filter-out $(FILTER_OUT_CPP), $(patsubst src/%, obj/%, $(CPP_SRC:.cpp=.o)))

DEP := $(CPP_OBJ:%.o=%.d)

.PHONY: all clean

all: main iso qemu

main: $(CRTI_OBJ) $(CRTBEGIN_OBJ) obj/boot.o obj/main.o $(CPP_OBJ) obj/Terminal.o $(NASM_OBJ) obj/tinyprintf.o $(CRTEND_OBJ) $(CRTN_OBJ)
	$(CXX) -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib $^ -lgcc

iso:
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub2-mkrescue -quiet -o myos.iso isodir

qemu:
	qemu-system-i386 -serial file:serial.log -cdrom myos.iso

qemu-gdb:
	qemu-system-i386 -serial file:serial.log -s -S -cdrom myos.iso

obj/%.o: src/%.nasm
	$(AS) -felf32 $< -o $@

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(OPTS) -c $< -o $@

obj/tinyprintf.o: src/tinyprintf.c
	$(CC) $(CCFLAGS) $(OPTS) -c $< -o $@

clean:
	rm -rf **/*.o **/*.d main myos.bin myos.iso isodir/ serial.log

-include $(DEP)
