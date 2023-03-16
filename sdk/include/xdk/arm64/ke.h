$if (_WDMDDK_)
/** Kernel definitions for ARM64 **/
typedef struct _KFLOATING_SAVE
{
    ULONG Reserved;
} KFLOATING_SAVE, *PKFLOATING_SAVE;
/* Interrupt request levels */
#define PASSIVE_LEVEL           0
#define LOW_LEVEL               0
#define APC_LEVEL               1
#define DISPATCH_LEVEL          2
#define CLOCK_LEVEL             13
#define IPI_LEVEL               14
#define DRS_LEVEL               14
#define POWER_LEVEL             14
#define PROFILE_LEVEL           15
#define HIGH_LEVEL              15

#define SharedUserData          ((KUSER_SHARED_DATA * const)KI_USER_SHARED_DATA)

#define PAGE_SIZE               0x1000
#define PAGE_SHIFT              12L

#define PAUSE_PROCESSOR YieldProcessor();

/* FIXME: Based on AMD64 but needed to compile apps */
#define KERNEL_STACK_SIZE                   12288
#define KERNEL_LARGE_STACK_SIZE             61440
#define KERNEL_LARGE_STACK_COMMIT KERNEL_STACK_SIZE
/* FIXME End */

#define EXCEPTION_READ_FAULT    0
#define EXCEPTION_WRITE_FAULT   1
#define EXCEPTION_EXECUTE_FAULT 8

NTSYSAPI
PKTHREAD
NTAPI
KeGetCurrentThread(VOID);

#define DbgRaiseAssertionFailure() __break(0xf001)

_IRQL_requires_max_(HIGH_LEVEL)
_IRQL_saves_
NTHALAPI
KIRQL
NTAPI
KeGetCurrentIrql(
    VOID);

_IRQL_requires_max_(HIGH_LEVEL)
NTHALAPI
VOID
FASTCALL
KfLowerIrql(
    _In_ _IRQL_restores_ _Notliteral_ KIRQL NewIrql);
#define KeLowerIrql(a) KfLowerIrql(a)

_IRQL_requires_max_(HIGH_LEVEL)
_IRQL_raises_(NewIrql)
_IRQL_saves_
NTHALAPI
KIRQL
FASTCALL
KfRaiseIrql(
    _In_ KIRQL NewIrql);
#define KeRaiseIrql(a,b) *(b) = KfRaiseIrql(a)

_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_saves_
_IRQL_raises_(DISPATCH_LEVEL)
NTHALAPI
KIRQL
NTAPI
KeRaiseIrqlToDpcLevel(VOID);

NTHALAPI
KIRQL
NTAPI
KeRaiseIrqlToSynchLevel(VOID);

_Requires_lock_not_held_(*SpinLock)
_Acquires_lock_(*SpinLock)
_IRQL_requires_max_(DISPATCH_LEVEL)
_IRQL_saves_
_IRQL_raises_(DISPATCH_LEVEL)
NTHALAPI
KIRQL
FASTCALL
KfAcquireSpinLock(
  _Inout_ PKSPIN_LOCK SpinLock);
#define KeAcquireSpinLock(a,b) *(b) = KfAcquireSpinLock(a)

_Requires_lock_held_(*SpinLock)
_Releases_lock_(*SpinLock)
_IRQL_requires_(DISPATCH_LEVEL)
NTHALAPI
VOID
FASTCALL
KfReleaseSpinLock(
  _Inout_ PKSPIN_LOCK SpinLock,
  _In_ _IRQL_restores_ KIRQL NewIrql);
#define KeReleaseSpinLock(a,b) KfReleaseSpinLock(a,b)

_Requires_lock_not_held_(*SpinLock)
_Acquires_lock_(*SpinLock)
_IRQL_requires_min_(DISPATCH_LEVEL)
NTKERNELAPI
VOID
FASTCALL
KefAcquireSpinLockAtDpcLevel(
  _Inout_ PKSPIN_LOCK SpinLock);
#define KeAcquireSpinLockAtDpcLevel(SpinLock) KefAcquireSpinLockAtDpcLevel(SpinLock)

_Requires_lock_held_(*SpinLock)
_Releases_lock_(*SpinLock)
_IRQL_requires_min_(DISPATCH_LEVEL)
NTKERNELAPI
VOID
FASTCALL
KefReleaseSpinLockFromDpcLevel(
  _Inout_ PKSPIN_LOCK SpinLock);
#define KeReleaseSpinLockFromDpcLevel(SpinLock) KefReleaseSpinLockFromDpcLevel(SpinLock)

NTSYSAPI
PKTHREAD
NTAPI
KeGetCurrentThread(VOID);

_Always_(_Post_satisfies_(return<=0))
_Must_inspect_result_
_IRQL_requires_max_(DISPATCH_LEVEL)
_Kernel_float_saved_
_At_(*FloatSave, _Kernel_requires_resource_not_held_(FloatState) _Kernel_acquires_resource_(FloatState))
FORCEINLINE
NTSTATUS
KeSaveFloatingPointState(
    _Out_ PKFLOATING_SAVE FloatSave)
{
    UNREFERENCED_PARAMETER(FloatSave);
    return STATUS_SUCCESS;
}

_Success_(1)
_Kernel_float_restored_
_At_(*FloatSave, _Kernel_requires_resource_held_(FloatState) _Kernel_releases_resource_(FloatState))
FORCEINLINE
NTSTATUS
KeRestoreFloatingPointState(
    _In_ PKFLOATING_SAVE FloatSave)
{
    UNREFERENCED_PARAMETER(FloatSave);
    return STATUS_SUCCESS;
}

VOID
KeFlushIoBuffers(
    _In_ PMDL Mdl,
    _In_ BOOLEAN ReadOperation,
    _In_ BOOLEAN DmaOperation);


$endif (_WDMDDK_)
$if (_NTDDK_)
#define ARM64_MAX_BREAKPOINTS 8
#define ARM64_MAX_WATCHPOINTS 2

typedef union NEON128 {
    struct {
        ULONGLONG Low;
        LONGLONG High;
    } DUMMYSTRUCTNAME;
    double D[2];
    float  S[4];
    USHORT H[8];
    UCHAR  B[16];
} NEON128, *PNEON128;
typedef NEON128 NEON128, *PNEON128;

typedef struct _CONTEXT {

    //
    // Control flags.
    //

    ULONG ContextFlags;

    //
    // Integer registers
    //

    ULONG Cpsr;
    union {
        struct {
            ULONG64 X0;
            ULONG64 X1;
            ULONG64 X2;
            ULONG64 X3;
            ULONG64 X4;
            ULONG64 X5;
            ULONG64 X6;
            ULONG64 X7;
            ULONG64 X8;
            ULONG64 X9;
            ULONG64 X10;
            ULONG64 X11;
            ULONG64 X12;
            ULONG64 X13;
            ULONG64 X14;
            ULONG64 X15;
            ULONG64 X16;
            ULONG64 X17;
            ULONG64 X18;
            ULONG64 X19;
            ULONG64 X20;
            ULONG64 X21;
            ULONG64 X22;
            ULONG64 X23;
            ULONG64 X24;
            ULONG64 X25;
            ULONG64 X26;
            ULONG64 X27;
            ULONG64 X28;
    		ULONG64 Fp;
            ULONG64 Lr;
        } DUMMYSTRUCTNAME;
        ULONG64 X[31];
    } DUMMYUNIONNAME;

    ULONG64 Sp;
    ULONG64 Pc;

    //
    // Floating Point/NEON Registers
    //

    NEON128 V[32];
    ULONG Fpcr;
    ULONG Fpsr;

    //
    // Debug registers
    //

    ULONG Bcr[ARM64_MAX_BREAKPOINTS];
    ULONG64 Bvr[ARM64_MAX_BREAKPOINTS];
    ULONG Wcr[ARM64_MAX_WATCHPOINTS];
    ULONG64 Wvr[ARM64_MAX_WATCHPOINTS];

} CONTEXT, *PCONTEXT;

typedef struct _KPCR
{
    _ANONYMOUS_UNION union
    {
        NT_TIB NtTib;
        _ANONYMOUS_STRUCT struct
        {
            ULONG TibPad0[2];
            PVOID Spare1;
            struct _KPCR *Self;
            struct _KPRCB *CurrentPrcb;
            PKSPIN_LOCK_QUEUE LockArray;
            PVOID Used_Self;
        };
    };
    KIRQL CurrentIrql;
    UCHAR SecondLevelCacheAssociativity;
    ULONG Unused0[3];
    USHORT MajorVersion;
    USHORT MinorVersion;
    ULONG StallScaleFactor;
    PVOID Unused1[3];
    ULONG KernelReserved[15];
    ULONG SecondLevelCacheSize;
    _ANONYMOUS_UNION union
    {
        USHORT SoftwareInterruptPending; // Software Interrupt Pending Flag
        struct
        {
            UCHAR ApcInterrupt;          // 0x01 if APC int pending
            UCHAR DispatchInterrupt;     // 0x01 if dispatch int pending
        };
    };
    USHORT InterruptPad;
    ULONG HalReserved[32];
    PVOID KdVersionBlock;
    PVOID Unused3;
    ULONG PcrAlign1[8];
} KPCR, *PKPCR;

FORCEINLINE
PKPCR
KeGetPcr(
    VOID)
{
    return (PKPCR)NULL;
}
$endif
