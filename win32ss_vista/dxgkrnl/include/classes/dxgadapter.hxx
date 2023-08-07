#pragma once

class DXGADAPTER
{
public:
    DXGADAPTER();
    ~DXGADAPTER();

    NTSTATUS Initialize(PDEVICE_OBJECT AdapterDeviceObject);
private:
};