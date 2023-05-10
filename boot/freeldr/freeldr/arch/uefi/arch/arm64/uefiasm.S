/*
 * PROJECT:     FreeLoader UEFI Support
 * LICENSE:     GPL-2.0-or-later (https://spdx.org/licenses/GPL-2.0-or-later)
 * PURPOSE:     UEFI assembly exit code
 * COPYRIGHT:   Copyright 2023 Justin Miller <justinmiller100@gmail.com>
 */
 #include <ksarm64.h>
    EXTERN EndofExitStack
    EXTERN Arm64MegaEnter
    NESTED_ENTRY _ChangeStack
    PROLOG_END _ChangeStack
    ldr x1, =EndofExitStack
    mov sp, x1
    bl Arm64MegaEnter
    NESTED_END _ChangeStack

//L_ArmInit:
//    .long ArmInit

    END