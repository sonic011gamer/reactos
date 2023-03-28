#include <dxgkrnl.h>
#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI 
DriverEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath)
{
    DPRINT1("---------------------------DXGKRNL Entry!---------------------------\n------------------------------------------------------------------\n");
   // __debugbreak();
    return TRUE;
}

NTSTATUS DxgkInitialize(
  _In_ PDRIVER_OBJECT              DriverObject,
  _In_ PUNICODE_STRING             RegistryPath,
  _In_ PDRIVER_INITIALIZATION_DATA DriverInitializationData
)
{
    return STATUS_SUCCESS;
}
// end DriverEntry()
