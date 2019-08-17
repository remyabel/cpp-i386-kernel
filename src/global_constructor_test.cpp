#include <Terminal.hpp>

/* This should segfault if global constructors are not
 * handled properly.
 */
class A {
  public:
    A() { printf("A()\n"); }
    void anything() { printf("No crash. Global constructors work.\n"); }
};

A g_a;

void global_constructor_test() {
    A *p_a = &g_a;
    p_a->anything(); // <---- segfault
}
