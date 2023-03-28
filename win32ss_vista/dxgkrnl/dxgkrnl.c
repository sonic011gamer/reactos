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
// end DriverEntry()
