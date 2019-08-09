/* Code taken from
 * https://wiki.osdev.org/C%2B%2B#Local_Static_Variables_.28GCC_Only.29 Modified
 * to implement the test-and-set primitive
 */
namespace __cxxabiv1 {
/* guard variables */

/* The ABI requires a 64-bit type.  */
__extension__ typedef int __guard __attribute__((mode(__DI__)));

void set_initialized(__guard *g) { *(char *)g = 1; }

bool is_initialized(__guard *g) {
    char *initialized = (char *)g;
    return *initialized;
}

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release(__guard *);
extern "C" void __cxa_guard_abort(__guard *);

extern "C" int __cxa_guard_acquire(__guard *g) { return !is_initialized(g); }

extern "C" void __cxa_guard_release(__guard *g) {
    *g = 0;
    set_initialized(g);
}

extern "C" void __cxa_guard_abort(__guard *g) { *g = 0; }
} // namespace __cxxabiv1
