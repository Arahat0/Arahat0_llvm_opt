#include <stdio.h>

void foo(int a) {
    int r0 = a + 0;
    int r1 = r0 * 16;
    int r2 = r1 * r0;
    int r3 = r2 / a;
    int r4 = r2 / 10;
    int r5 = 54 * r3;
    int r6 = r4 / 128;
    int r7 = r5 / 54;
    int r8 = r4 / 1;
    int r9 = r7 - 0;
    printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", r0, r1, r2, r3, r4, r5, r6, r7, r8, r9);
}