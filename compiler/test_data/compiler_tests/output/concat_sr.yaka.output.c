// YK
#include "yk__lib.h"
// --forward declarations-- 
struct yk__bstr const yy__A = ((struct yk__bstr){.data.s = " Hello", .l = 6, .t = yk__bstr_static});
int32_t yy__do_something(struct yk__bstr);
int32_t yy__takes_str(yk__sds);
int32_t yy__main();
// --structs-- 
// --functions-- 
int32_t yy__do_something(struct yk__bstr yy__s) 
{
    yk__printstr("Printing sr: ");
    yk__printlnstr(yk__bstr_get_reference(yy__s));
    return INT32_C(0);
}
int32_t yy__takes_str(yk__sds yy__s) 
{
    yk__printstr("Printing str: ");
    yk__printlnstr(yy__s);
    yk__sdsfree(yy__s);
    return INT32_C(0);
}
int32_t yy__main() 
{
    struct yk__bstr yy__a = yk__bstr_s("Hi" , 2);
    struct yk__bstr yy__b = yk__bstr_s(" there" , 6);
    yk__sds t__0 = yk__concat_bstr_bstr(yy__a, yy__b);
    yk__sds t__1 = yk__concat_sds_bstr(t__0, yy__a);
    yy__do_something(yk__bstr_h(t__1));
    yk__sds t__2 = yk__concat_bstr_bstr(yy__a, yy__b);
    yy__takes_str(yk__sdsdup(t__2));
    yk__sds t__3 = yk__concat_bstr_bstr(yy__a, yy__b);
    yk__sds t__4 = yk__concat_sds_bstr(t__3, yy__A);
    yy__takes_str(yk__sdsdup(t__4));
    yk__sdsfree(t__4);
    yk__sdsfree(t__3);
    yk__sdsfree(t__2);
    yk__sdsfree(t__1);
    yk__sdsfree(t__0);
    return INT32_C(0);
}
#if defined(YK__MINIMAL_MAIN)
int main(void) { return yy__main(); }
#endif