
#include <include/dxgkrnl.h>
#include <debug.h>


DXGADAPTER::DXGADAPTER()
{

}

DXGADAPTER::~DXGADAPTER()
{

}

NTSTATUS
DXGADAPTER::Initialize(PDEVICE_OBJECT AdapterDeviceObject)
{
    if (!AdapterDeviceObject)
        return STATUS_INVALID_PARAMETER;

    UNIMPLEMENTED;
    __debugbreak();


    /*
     * What a chonker.
     * A BUNCH of classes are initialized here it seems
     * let's go over what's important.
     *
     * 1)
     */
    return STATUS_UNSUCCESSFUL;
}