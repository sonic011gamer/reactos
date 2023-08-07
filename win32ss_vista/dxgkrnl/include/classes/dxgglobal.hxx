#pragma once

class DXGGLOBAL
{
public:
    DXGGLOBAL();
    ~DXGGLOBAL(){}
    DXGGLOBAL* GetGlobal(VOID);

    NTSTATUS CreateGlobal(VOID);
    NTSTATUS DestroyGlobal(VOID);
    BOOLEAN CheckExclusiveOwnership(VOID);
    DXGADAPTER ReferenceAdapterByLuid(LUID TargetLuid);
    VOID ReportState(VOID);
    NTSTATUS CreateAdapter(PDEVICE_OBJECT AdapterDeviceObject, DXGADAPTER **DxgAdapter);
    //TODO: - Needs reversing
    //BOOLEAN IsWindowVisible(?)
    //VOID ControlVSyncForLogging(?)
private:
    DXGADAPTER* GlobalAdapterList; /* HACK: only a single adapter*/
};