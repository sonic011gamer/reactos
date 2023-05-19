/*
 * PROJECT:     FreeLoader UEFI Support
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     UEFI assembly exit code
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */
 #include <ksarm64.h>

    EXTERN EndofExitStack
    EXTERN PgdBase
    EXTERN ARM64IsAwesome
    EXTERN Arm64MegaEnter

    NESTED_ENTRY _ChangeStack
    PROLOG_END _ChangeStack
    ldr x1, =EndofExitStack
    mov sp, x1
    bl Arm64MegaEnter
    NESTED_END _ChangeStack

    /* Get Current exception level */
    NESTED_ENTRY KiGetCurrentExceptionLevel
    PROLOG_END KiGetCurrentExceptionLevel
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret
    NESTED_END KiGetCurrentExceptionLevel

    /* void KiEnableMmu(void) */
    NESTED_ENTRY KiEnableMmu
    PROLOG_END KiEnableMmu
    mrs	x1, sctlr_el1
	bic	x1, x1, #1
	msr	sctlr_el1, x1
    adrp x0, PgdBase
    msr ttbr1_el1, x0
    mrs X0, SCTLR_EL1
    orr X0, X0, #0x1 // The M bit (MMU).
    msr SCTLR_EL1, X0
    bl KiTestUART
    bl ARM64IsAwesome
    NESTED_END KiEnableMmu

    NESTED_ENTRY KiTestUART
    PROLOG_END KiTestUART
    mov x1, 0x09000000
    mov X2, 'C'
    str x2, [x1]
    ret
    NESTED_END KiTestUART

    /* void KiDisableMmu(void) */
    NESTED_ENTRY KiDisableMmu
    PROLOG_END KiDisableMmu
    mrs	x1, sctlr_el1
	bic	x1, x1, #1
	msr	sctlr_el1, x1			// disable the MMU
	isb
	NESTED_END KiEnableMmu

    END