// YK
#include "yk__lib.h"
// --forward declarations-- 
int32_t yy__main();
// --structs-- 
// --functions-- 
int32_t yy__main() 
{
    yk__printlnstr("Creating a fixed array");
    int32_t yy__a[3] = {};
    yy__a[INT32_C(0)] = INT32_C(1);
    yy__a[INT32_C(1)] = INT32_C(2);
    yy__a[INT32_C(2)] = INT32_C(3);
    int32_t yy__b = ((yy__a[INT32_C(0)] + yy__a[INT32_C(1)]) + yy__a[INT32_C(2)]);
    yk__printlnint((intmax_t)yy__b);
    return INT32_C(0);
}
#if defined(YK__MINIMAL_MAIN)
int main(void) { return yy__main(); }
#endif