#include <stdio.h>

inline void print_float(float n)
{
    unsigned short d1 = (unsigned short) n;
    unsigned short d2 = (n - d1) * 1000;
    printf("%u.%03u\n", d1, d2);
}
