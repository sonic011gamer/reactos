
/* INCLUDES ******************************************************************/

/* We need one of these first! */
/* #include <kxarm64.h> */

/* CODE **********************************************************************/
    TEXTAREA

    LEAF_ENTRY __chkstk
        lsl    x16, x15, #4
        mov    x17, sp
1:
        ldr    xzr, [x17]

        ret
    LEAF_END __chkstk

    LEAF_ENTRY __alloca_probe
    /* TODO: add an assert fail call, as this is unimplemented */
    LEAF_END __alloca_probe

    END
/* EOF */
