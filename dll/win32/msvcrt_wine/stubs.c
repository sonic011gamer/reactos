#include <stubs.h>
#include <debug.h>
#undef UNIMPLEMENTED
#define UNIMPLEMENTED __wine_spec_unimplemented_stub("msvcrt.dll", __FUNCTION__)

cdecl 
VOID
_libm_sse2_sqrt_precise()
{
    UNIMPLEMENTED;
    return;
}