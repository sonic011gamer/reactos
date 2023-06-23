#pragma once
/* Internal versions of public function pointers */
NTSTATUS
APIENTRY
RDDM_DxgkCbEvalAcpiMethod(_In_ HANDLE DeviceHandle,
                          _In_ ULONG DeviceUid,
                          _In_reads_bytes_(AcpiInputSize) PACPI_EVAL_INPUT_BUFFER_COMPLEX AcpiInputBuffer,
                          _In_range_(>=, sizeof(ACPI_EVAL_INPUT_BUFFER_COMPLEX)) ULONG AcpiInputSize,
                          _Out_writes_bytes_(AcpiOutputSize) PACPI_EVAL_OUTPUT_BUFFER AcpiOutputBuffer,
                          _In_range_(>=, sizeof(ACPI_EVAL_OUTPUT_BUFFER)) ULONG AcpiOutputSize);

NTSTATUS
APIENTRY
RDDM_DxgkCbGetDeviceInformation(_In_ HANDLE DeviceHandle,
                                _Out_ PDXGK_DEVICE_INFO DeviceInfo);

NTSTATUS
APIENTRY
RDDM_DxgkCbIndicateChildStatus(_In_ HANDLE DeviceHandle,
                               _In_ PDXGK_CHILD_STATUS ChildStatus);


NTSTATUS
APIENTRY
RDDM_DxgkCbMapMemory(_In_ HANDLE DeviceHandle,
                     _In_ PHYSICAL_ADDRESS TranslatedAddress,
                     _In_ ULONG Length,
                     _In_ BOOLEAN InIoSpace,
                     _In_ BOOLEAN MapToUserMode,
                     _In_ MEMORY_CACHING_TYPE CacheType,
                     _Outptr_ PVOID *VirtualAddress);

BOOLEAN
APIENTRY
RDDM_DxgkCbQueueDpc(_In_ HANDLE DeviceHandle);


NTSTATUS
APIENTRY
RDDM_DxgkCbQueryServices(_In_ HANDLE DeviceHandle,
                         _In_ DXGK_SERVICES ServicesType,
                         _Inout_ PINTERFACE Interface);

NTSTATUS
APIENTRY
RDDM_DxgkCbReadDeviceSpace(_In_ HANDLE DeviceHandle,
                           _In_ ULONG DataType,
                           _Out_writes_bytes_to_(Length, *BytesRead) PVOID Buffer,
                           _In_ ULONG Offset,
                           _In_ ULONG Length,
                           _Out_ PULONG BytesRead);

NTSTATUS
APIENTRY
RDDM_DxgkCbSynchronizeExecution(_In_ HANDLE DeviceHandle,
                                _In_ PKSYNCHRONIZE_ROUTINE SynchronizeRoutine,
                                _In_ PVOID Context,
                                _In_ ULONG MessageNumber,
                                _Out_ PBOOLEAN ReturnValue);

NTSTATUS
APIENTRY
RDDM_DxgkCbUnmapMemory(_In_ HANDLE DeviceHandle,
                       _In_ PVOID VirtualAddress);

NTSTATUS
APIENTRY
RDDM_DxgkCbWriteDeviceSpace(_In_ HANDLE DeviceHandle,
                            _In_ ULONG DataType,
                            _In_reads_bytes_(Length) PVOID Buffer,
                            _In_ ULONG Offset,
                            _In_ ULONG Length,
                            _Out_ _Out_range_(<=, Length) PULONG BytesWritten);

NTSTATUS
APIENTRY
RDDM_DxgkCbIsDevicePresent(_In_ HANDLE DeviceHandle,
                           _In_ PPCI_DEVICE_PRESENCE_PARAMETERS DevicePresenceParameters,
                           _Out_ PBOOLEAN DevicePresent);
VOID*
APIENTRY
CALLBACK
RDDM_DxgkCbGetHandleData(IN_CONST_PDXGKARGCB_GETHANDLEDATA GetHandleData);

D3DKMT_HANDLE
APIENTRY
CALLBACK
RDDM_DxgkCbGetHandleParent(IN_D3DKMT_HANDLE hAllocation);

D3DKMT_HANDLE
APIENTRY
CALLBACK
RDDM_DxgkCbEnumHandleChildren(IN_CONST_PDXGKARGCB_ENUMHANDLECHILDREN EnumHandleChildren);

VOID
APIENTRY
CALLBACK
RDDM_DxgkCbNotifyInterrupt(IN_CONST_HANDLE hAdapter,
                           IN_CONST_PDXGKARGCB_NOTIFY_INTERRUPT_DATA NotifyInterruptData);
VOID
APIENTRY
CALLBACK
RDDM_DxgkCbNotifyDpc(IN_CONST_HANDLE hAdapter);

NTSTATUS
APIENTRY
CALLBACK
RDDM_DxgkCbQueryVidPnInterface(IN_CONST_D3DKMDT_HVIDPN                hVidPn,
                               IN_CONST_DXGK_VIDPN_INTERFACE_VERSION  VidPnInterfaceVersion,
                               DEREF_OUT_CONST_PPDXGK_VIDPN_INTERFACE ppVidPnInterface);
NTSTATUS
APIENTRY
CALLBACK
RDDM_DxgkCbQueryMonitorInterface(IN_CONST_HANDLE                          hAdapter,
                                 IN_CONST_DXGK_MONITOR_INTERFACE_VERSION  MonitorInterfaceVersion,
                                 DEREF_OUT_CONST_PPDXGK_MONITOR_INTERFACE ppMonitorInterface);

NTSTATUS
APIENTRY
CALLBACK
RDDM_DxgkCbGetCaptureAddress(INOUT_PDXGKARGCB_GETCAPTUREADDRESS GetCaptureAddress);

VOID
APIENTRY
RDDM_DxgkCbLogEtwEvent(_In_ CONST LPCGUID EventGuid,
                       _In_ UCHAR Type,
                       _In_ USHORT EventBufferSize,
                       _In_reads_bytes_(EventBufferSize) PVOID EventBuffer);
