

#include <stubs.h>

#undef UNIMPLEMENTED
#define UNIMPLEMENTED __wine_spec_unimplemented_stub("msvcrt.dll", __FUNCTION__)

#include <stdio.h>
#include <wine/asm.h>
extern FILE * __cdecl __iob_func(void);
FILE * __cdecl __acrt_iob_func(unsigned idx)
{
    return __iob_func() + idx;
}
