CXX = i686-elf-g++
AS = i686-elf-as
CXXFLAGS = -Wall -Wextra -O2 -pedantic -ffreestanding -fno-exceptions -fno-rtti
OPTS = -MMD -MP -Iinclude

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%, obj/%, $(SRC:.cpp=.o))

TESTSRC := $(wildcard test/*.cpp)
TESTOBJ := $(patsubst test/%.cpp, test/%.o, $(TESTSRC:.cpp=.o))

all: main test iso qemu

main: $(OBJ) obj/boot.o
	$(CXX) -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib $^ -lgcc

iso:
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub2-mkrescue -o myos.iso isodir

qemu:
	qemu-system-i386 -cdrom myos.iso

test: $(TESTOBJ) $(filter-out obj/main.o, $(OBJ))
	g++ -O2 $^ -o test/test-main
	test/test-main -r compact

obj/boot.o: src/boot.s
	$(AS) $< -o $@

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(OPTS) -c $< -o $@

test/%.o: test/%.cpp
	g++ -O2 -Wall -Wextra -pedantic $(OPTS) -c $< -o $@

clean:
	rm -rf obj/* main test/test-main test/*.o test/*.d myos* isodir/

-include $(OBJ:%.o=%.d)
-include $(TESTOBJ:%.o=%.d)

.PHONY: clean
