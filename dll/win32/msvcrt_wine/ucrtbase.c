#include <stubs.h>
#include <precomp.h>
#include "msvcrt.h"
#include <debug.h>

#undef UNIMPLEMENTED
#define UNIMPLEMENTED __wine_spec_unimplemented_stub("msvcrt.dll", __FUNCTION__)
#define MSVCRT__RC_CHOP       0x00000300
#define MSVCRT__RC_UP         0x00000200
#define MSVCRT__RC_NEAR       0x00000000

/*
 * @implemented
 */

int CDECL __fpe_flt_rounds(void)
{
    return 2;
}