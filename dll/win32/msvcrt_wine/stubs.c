#include <stubs.h>
#include <debug.h>
#undef UNIMPLEMENTED
#define UNIMPLEMENTED __wine_spec_unimplemented_stub("msvcrt.dll", __FUNCTION__)

/*
 * These are kind of a nuisence, i'll have to implement later from wine. hence their location
 */
void cdecl __libm_sse2_sqrt_precise(void)
{
    UNIMPLEMENTED;
    return;
}


void cdecl __libm_sse2_acos_precise(void)
{
    UNIMPLEMENTED;
    return;
}

void cdecl __libm_sse2_asin_precise(void)
{
    UNIMPLEMENTED;
    return;
}

void cdecl __libm_sse2_atan_precise(void)
{
    UNIMPLEMENTED;
    return;
}

void cdecl __libm_sse2_cos_precise(void)
{
    UNIMPLEMENTED;
    return;
}

void cdecl __libm_sse2_exp_precise(void) 
{
    UNIMPLEMENTED;
    return;
}

void cdecl __libm_sse2_log10_precise(void)
{
    UNIMPLEMENTED;
    return;
}

void cdecl __libm_sse2_log_precise(void)
{
    UNIMPLEMENTED;
    return;
}

void cdecl __libm_sse2_pow_precise(void)
{
    UNIMPLEMENTED;
    return;
}

void cdecl __libm_sse2_sin_precise(void)
{
    UNIMPLEMENTED;
    return;
}
