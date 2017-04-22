# Bare Bones i386 kernel

## Impetus

C is a popular language for embedded systems and there is a lot of myths that make C++ an unpopular
choice.

[CppCon 2016: Jason Turner “Rich Code for Tiny Computers: A Simple Commodore 64 Game in C++17”](https://www.youtube.com/watch?v=zBkNBP00wJE)

The above talk shows how C++17 features add **exactly zero** overhead when used correctly.

[CppCon 2016: Dan Saks “extern c: Talking to C Programmers about C++”](https://www.youtube.com/watch?v=D7Sd8A6_fYU)

The above is a more opinionated talk that addresses the schism between the C and C++ community in the embedded world.

## Info

This project spawned from [OSDev Bare Bones tutorial](http://wiki.osdev.org/Bare_Bones), but has taken a life of its own.

### Voodoo

Keep in mind while the advice there is good, the code sometimes contains items of questionable benefit. For example:

```C++
static inline foo();
```

When putting this in a header file (which many people do), the `static` keyword will simply add bloat to the executable size because there is a separate `foo()` for every translation unit. 

```C++
outb(0x3D4, position & 0xFF);
```

Here masking is used to get the least significant byte. This contains a (potential) implicit conversion to an `unsigned` type; but if you are already working with an `unsigned` type, then masking is not necessary. This is more readable code:

```C++
static_cast<uint8_t>(position);
```

### Mini standard library
- [x] Use `string_view` instead of naked `const char*`
- [x] Put Terminal in its own class
- [ ] Implement `span` (read/write view) for video buffer
- [ ] Implement `memcpy` and other useful C library functions (is this needed? `copy` and `string_view` do just fine)
- [x] Implement iterators (for random access iterator)
    - [ ] then add an iterator interface to Terminal/spans

See issues for checklist on various standard library stuff (`array`, etc.)

### VESA
- [ ] Integrate TTF font (using tool that converts the font to a C file)

### Kernel stuff
- [ ] IDT
- [ ] GDT
- [ ] PMM
- [ ] VMM

### Build system

The build system I am using is GCC 6.3.0 with Binutils 2.28 and NASM (the OSDev tutorials use GAS syntax)

The C++ testing framework is [Catch](https://github.com/philsquared/Catch). 

### Make commands

- **make test** Runs Catch in compact mode
- **make qemu** Launches `qemu-system-i386`

## Roadmap

- Use GSL features like `span` (container-agnostic view ala `string_view`)

- Implement a physical memory manager using a Bitmap allocation algorithm (slow but simple)

- Use Boyer-Moore for string searching

- Use BST's for many situations
