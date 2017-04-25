CXX = i686-elf-g++
AS = nasm
CXXFLAGS = -Wall -Wextra -O2 -pedantic -ffreestanding -fno-exceptions -fno-rtti
OPTS = -MD -MP -Iinclude

CRTI_OBJ = obj/crti.o
CRTBEGIN_OBJ := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ := $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtend.o)
CRTN_OBJ = obj/crtn.o

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%, obj/%, $(SRC:.cpp=.o))

TESTSRC := $(wildcard test/*.cpp)
TESTOBJ := $(patsubst test/%.cpp, test/%.o, $(TESTSRC:.cpp=.o))

DEP := $(OBJ:%.o=%.d)
TESTDEP := $(TESTOBJ:%.o=%.d)

.PHONY: all clean

all: main test iso qemu

main: $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJ) obj/boot.o $(CRTEND_OBJ) $(CRTN_OBJ)
	$(CXX) -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib $^ -lgcc

iso:
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub2-mkrescue -quiet -o myos.iso isodir

qemu:
	qemu-system-i386 -cdrom myos.iso

test: $(TESTOBJ)
	g++ -O2 $^ -o test/test-main
	test/test-main -r compact

obj/crti.o: src/crti.s
	$(AS) -felf32 $< -o $@

obj/crtn.o: src/crtn.s
	$(AS) -felf32 $< -o $@

obj/boot.o: src/boot.s
	$(AS) -felf32 $< -o $@

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(OPTS) -c $< -o $@

test/%.o: test/%.cpp
	g++ -O2 -Wall -Wextra -pedantic $(OPTS) -c $< -o $@

clean:
	rm -rf obj/* main test/test-main test/*.o test/*.d myos* isodir/

-include $(DEP)
-include $(TESTDEP)
