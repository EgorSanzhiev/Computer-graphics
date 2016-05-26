#include "intmath.h"

int IntMath::power(int a, int b) {
    int result = 1;

    for (int i = 0; i < b; ++i)
        result *= a;

    return result;
}

int IntMath::abs(int x)
{
    return x > 0 ? x : -x;
}
