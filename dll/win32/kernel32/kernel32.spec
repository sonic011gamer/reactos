@ stdcall -version=0x600+ AcquireSRWLockExclusive(ptr) NTDLL.RtlAcquireSRWLockExclusive
@ stdcall -version=0x600+ AcquireSRWLockShared(ptr) NTDLL.RtlAcquireSRWLockShared
@ stdcall ActivateActCtx(ptr ptr)
@ stdcall AddAtomA(str)
@ stdcall AddAtomW(wstr)
@ stdcall AddConsoleAliasA(str str str) ;check
@ stdcall AddConsoleAliasW(wstr wstr wstr) ;check
@ stdcall AddLocalAlternateComputerNameA(str ptr)
@ stdcall AddLocalAlternateComputerNameW(wstr ptr)
@ stdcall AddRefActCtx(ptr)
@ stdcall -stub -version=0x600+ AddSIDToBoundaryDescriptor(ptr ptr)
@ stdcall -stub -version=0x600+ AddSecureMemoryCacheCallback(ptr)
@ stdcall AddVectoredContinueHandler(long ptr) ntdll.RtlAddVectoredContinueHandler
@ stdcall AddVectoredExceptionHandler(long ptr) ntdll.RtlAddVectoredExceptionHandler
@ stdcall -stub -version=0x600+ AdjustCalendarDate(ptr long long)
@ stdcall AllocConsole()
@ stub ResolveLocaleName
@ stdcall AllocateUserPhysicalPages(long ptr ptr)
@ stdcall -stub -version=0x600+ AllocateUserPhysicalPagesNuma(ptr ptr ptr long)
@ stdcall -version=0x600+ ApplicationRecoveryFinished(long)
@ stdcall -version=0x600+ ApplicationRecoveryInProgress(ptr)
@ stdcall AreFileApisANSI()
@ stdcall AssignProcessToJobObject(ptr ptr)
@ stdcall AttachConsole(long)
@ stdcall BackupRead(ptr ptr long ptr long long ptr)
@ stdcall BackupSeek(ptr long long ptr ptr ptr)
@ stdcall BackupWrite(ptr ptr long ptr long long ptr)
@ stdcall BaseCheckAppcompatCache(long long long ptr)
@ stdcall BaseCheckRunApp(long ptr long long long long long long long long)
@ stdcall BaseCleanupAppcompatCacheSupport(ptr)
@ stdcall BaseDumpAppcompatCache()
@ stdcall BaseFlushAppcompatCache()
@ stub -version=0x600+ BaseGenerateAppCompatData
@ stdcall -version=0x601+ BaseGetNamedObjectDirectory(ptr)
@ stdcall BaseInitAppcompatCacheSupport()
@ stdcall BaseIsAppcompatInfrastructureDisabled() IsShimInfrastructureDisabled
@ stdcall -version=0x501-0x502 BaseProcessInitPostImport()
;@ stdcall -version=0x502 -arch=x86_64 BaseProcessStart()
@ stdcall BaseQueryModuleData(str str ptr ptr ptr) ;check
@ stub -version=0x600+ BaseThreadInitThunk
;@ stdcall -version=0x502 -arch=x86_64 BaseThreadStart()
@ stdcall BaseUpdateAppcompatCache(long long long)
@ stdcall BasepCheckBadapp(long ptr long long long long long long long)
@ stdcall BasepCheckWinSaferRestrictions(long long long long long long)
@ stdcall BasepFreeAppCompatData(ptr ptr)
@ stdcall Beep(long long)
@ stdcall BeginUpdateResourceA(str long)
@ stdcall BeginUpdateResourceW(wstr long)
@ stdcall BindIoCompletionCallback(long ptr long)
@ stdcall BuildCommDCBA(str ptr)
@ stdcall BuildCommDCBAndTimeoutsA(str ptr ptr)
@ stdcall BuildCommDCBAndTimeoutsW(wstr ptr ptr)
@ stdcall BuildCommDCBW(wstr ptr)
@ stdcall CallNamedPipeA(str ptr long ptr long ptr long)
@ stdcall CallNamedPipeW(wstr ptr long ptr long ptr long)
@ stdcall -stub -version=0x600+ CallbackMayRunLong(ptr)
@ stdcall CancelDeviceWakeupRequest(long)
@ stdcall CancelIo(long)
@ stdcall -stub -version=0x600+ CancelIoEx(ptr ptr)
@ stdcall -stub -version=0x600+ CancelSynchronousIo(ptr)
@ stdcall -stub -version=0x600+ CancelThreadpoolIo(ptr)
@ stdcall CancelTimerQueueTimer(long long)
@ stdcall CancelWaitableTimer(long)
@ stdcall ChangeTimerQueueTimer(ptr ptr long long)
@ stdcall -stub -version=0x600+ CheckElevation(ptr ptr ptr ptr ptr)
@ stdcall -stub -version=0x600+ CheckElevationEnabled(ptr)
@ stub -version=0x600+ CheckForReadOnlyResource
@ stdcall CheckNameLegalDOS8Dot3A(str str long long long)
@ stdcall CheckNameLegalDOS8Dot3W(wstr str long long long)
@ stdcall CheckRemoteDebuggerPresent(long ptr)
@ stdcall ClearCommBreak(long)
@ stdcall ClearCommError(long ptr ptr)
@ stdcall CloseConsoleHandle(long)
@ stdcall CloseHandle(long)
@ stdcall -stub -version=0x600+ ClosePrivateNamespace(ptr long)
@ stdcall CloseProfileUserMapping()
@ stdcall -stub -version=0x600+ CloseThreadpool(ptr)
@ stdcall -stub -version=0x600+ CloseThreadpoolCleanupGroup(ptr)
@ stdcall -stub -version=0x600+ CloseThreadpoolCleanupGroupMembers(ptr long ptr)
@ stdcall -stub -version=0x600+ CloseThreadpoolIo(ptr)
@ stdcall -stub -version=0x600+ CloseThreadpoolTimer(ptr)
@ stdcall -stub -version=0x600+ CloseThreadpoolWait(ptr)
@ stdcall -stub -version=0x600+ CloseThreadpoolWork(ptr)
@ stdcall CmdBatNotification(long)
@ stdcall CommConfigDialogA(str long ptr)
@ stdcall CommConfigDialogW(wstr long ptr)
@ stdcall -stub -version=0x600+ CompareCalendarDates(ptr ptr ptr)
@ stdcall CompareFileTime(ptr ptr)
@ stdcall CompareStringA(long long str long str long)
@ stdcall -version=0x600+ CompareStringEx(wstr long wstr long wstr long ptr ptr ptr)
@ stdcall -version=0x600+ CompareStringOrdinal(wstr long wstr long long) kernel32_vista.CompareStringOrdinal
@ stdcall CompareStringW(long long wstr long wstr long)
@ stdcall ConnectNamedPipe(long ptr)
;@ stdcall -arch=x86_64 ConsoleIMERoutine()
@ stdcall ConsoleMenuControl(long long long)
@ stdcall ContinueDebugEvent(long long long)
@ stdcall -stub -version=0x600+ ConvertCalDateTimeToSystemTime(ptr ptr)
@ stdcall ConvertDefaultLocale (long)
@ stdcall ConvertFiberToThread()
@ stdcall -stub -version=0x600+ ConvertNLSDayOfWeekToWin32DayOfWeek(long)
@ stdcall -stub -version=0x600+ ConvertSystemTimeToCalDateTime(ptr long ptr)
@ stdcall ConvertThreadToFiber(ptr)
@ stdcall ConvertThreadToFiberEx(ptr long)
@ stdcall CopyFileA(str str long)
@ stdcall CopyFileExA (str str ptr ptr ptr long)
@ stdcall CopyFileExW (wstr wstr ptr ptr ptr long)
@ stdcall -stub -version=0x600+ CopyFileTransactedA(str str ptr ptr ptr long ptr)
@ stdcall -stub -version=0x600+ CopyFileTransactedW(wstr wstr ptr ptr ptr long ptr)
@ stdcall CopyFileW(wstr wstr long)
@ stdcall CopyLZFile(long long) LZCopy
@ stdcall CreateActCtxA(ptr)
@ stdcall CreateActCtxW(ptr)
@ stdcall -stub -version=0x600+ CreateBoundaryDescriptorA(str long)
@ stdcall -stub -version=0x600+ CreateBoundaryDescriptorW(wstr long)
@ stdcall CreateConsoleScreenBuffer(long long ptr long ptr)
@ stdcall CreateDirectoryA(str ptr)
@ stdcall CreateDirectoryExA(str str ptr)
@ stdcall CreateDirectoryExW(wstr wstr ptr)
@ stdcall -stub -version=0x600+ CreateDirectoryTransactedA(str str ptr ptr)
@ stdcall -stub -version=0x600+ CreateDirectoryTransactedW(wstr wstr ptr ptr)
@ stdcall CreateDirectoryW(wstr ptr)
@ stdcall CreateEventA(ptr long long str)
@ stdcall -version=0x600+ CreateEventExA(ptr str long long)
@ stdcall -version=0x600+ CreateEventExW(ptr wstr long long)
@ stdcall CreateEventW(ptr long long wstr)
@ stdcall CreateFiber(long ptr ptr)
@ stdcall CreateFiberEx(long long long ptr ptr)
@ stdcall CreateFileA(str long long ptr long long long)
@ stdcall CreateFileMappingA(long ptr long long long str)
@ stdcall -stub -version=0x600+ CreateFileMappingNumaA(ptr ptr long long long str long)
@ stdcall -stub -version=0x600+ CreateFileMappingNumaW(ptr ptr long long long wstr long)
@ stdcall CreateFileMappingW(long ptr long long long wstr)
@ stdcall -stub -version=0x600+ CreateFileTransactedA(str long long ptr long long ptr ptr ptr ptr)
@ stdcall -stub -version=0x600+ CreateFileTransactedW(wstr long long ptr long long ptr ptr ptr ptr)
@ stdcall CreateFileW(wstr long long ptr long long long)
@ stdcall CreateHardLinkA(str str ptr)
@ stdcall -stub -version=0x600+ CreateHardLinkTransactedA(str str ptr ptr)
@ stdcall -stub -version=0x600+ CreateHardLinkTransactedW(wstr wstr ptr ptr)
@ stdcall CreateHardLinkW(wstr wstr ptr)
@ stdcall CreateIoCompletionPort(long long long long)
@ stdcall CreateJobObjectA(ptr str)
@ stdcall CreateJobObjectW(ptr wstr)
@ stdcall CreateJobSet(long ptr long)
@ stdcall CreateMailslotA(ptr long long ptr)
@ stdcall CreateMailslotW(ptr long long ptr)
@ stdcall CreateMemoryResourceNotification(long)
@ stdcall CreateMutexA(ptr long str)
@ stdcall -version=0x600+ CreateMutexExA(ptr str long long)
@ stdcall -version=0x600+ CreateMutexExW(ptr wstr long long)
@ stdcall CreateMutexW(ptr long wstr)
@ stdcall CreateNamedPipeA(str long long long long long long ptr)
@ stdcall CreateNamedPipeW(wstr long long long long long long ptr)
@ stdcall -version=0x501-0x502 CreateNlsSecurityDescriptor(ptr long long)
@ stdcall CreatePipe(ptr ptr ptr long)
@ stdcall -stub -version=0x600+ CreatePrivateNamespaceA(ptr ptr str)
@ stdcall -stub -version=0x600+ CreatePrivateNamespaceW(ptr ptr wstr)
@ stdcall CreateProcessA(str str ptr ptr long long ptr str ptr ptr)
@ stdcall CreateProcessInternalA(ptr str str ptr ptr long long ptr str ptr ptr long)
@ stdcall CreateProcessInternalW(ptr wstr wstr ptr ptr long long ptr wstr ptr ptr long)
@ stdcall CreateProcessW(wstr wstr ptr ptr long long ptr wstr ptr ptr)
@ stdcall CreateRemoteThread(long ptr long ptr long long ptr)
@ stdcall CreateSemaphoreA(ptr long long str)
@ stdcall -version=0x600+ CreateSemaphoreExA(ptr long long str long long)
@ stdcall -version=0x600+ CreateSemaphoreExW(ptr long long wstr long long)
@ stdcall CreateSemaphoreW(ptr long long wstr)
@ stdcall -i386 CreateSocketHandle()
@ stdcall -version=0x600+ CreateSymbolicLinkA(str str long)
@ stdcall -stub -version=0x600+ CreateSymbolicLinkTransactedA(str str long ptr)
@ stdcall -stub -version=0x600+ CreateSymbolicLinkTransactedW(wstr wstr long ptr)
@ stdcall -version=0x600+ CreateSymbolicLinkW(wstr wstr long)
@ stdcall CreateTapePartition(long long long long)
@ stdcall CreateThread(ptr long ptr long long ptr)
@ stdcall CreateTimerQueue ()
@ stdcall CreateTimerQueueTimer(ptr long ptr ptr long long long)
@ stdcall -stub -version=0x600+ CreateThreadpool(ptr)
@ stdcall -stub -version=0x600+ CreateThreadpoolCleanupGroup()
@ stdcall -stub -version=0x600+ CreateThreadpoolIo(ptr ptr ptr ptr)
@ stdcall -stub -version=0x600+ CreateThreadpoolTimer(ptr ptr ptr)
@ stdcall -stub -version=0x600+ CreateThreadpoolWait(ptr ptr ptr)
@ stdcall -stub -version=0x600+ CreateThreadpoolWork(ptr ptr ptr)
@ stdcall CreateToolhelp32Snapshot(long long)
@ stdcall CreateWaitableTimerA(ptr long str)
@ stub -version=0x600+ CreateWaitableTimerExA
@ stub -version=0x600+ CreateWaitableTimerExW
@ stdcall CreateWaitableTimerW(ptr long wstr)
;@ stdcall -arch=x86_64 CtrlRoutine()
@ stdcall DeactivateActCtx(long ptr)
@ stdcall DebugActiveProcess(long)
@ stdcall DebugActiveProcessStop(long)
@ stdcall DebugBreak() ntdll.DbgBreakPoint
@ stdcall DebugBreakProcess(long)
@ stdcall DebugSetProcessKillOnExit(long)
@ stdcall DecodePointer(ptr) ntdll.RtlDecodePointer
@ stdcall DecodeSystemPointer(ptr) ntdll.RtlDecodeSystemPointer
@ stdcall DefineDosDeviceA(long str str)
@ stdcall DefineDosDeviceW(long wstr wstr)
@ stdcall DelayLoadFailureHook(str str)
@ stdcall DeleteAtom(long)
@ stub -version=0x600+ DeleteBoundaryDescriptor
@ stdcall DeleteCriticalSection(ptr) ntdll.RtlDeleteCriticalSection
@ stdcall DeleteFiber(ptr)
@ stdcall DeleteFileA(str)
@ stub -version=0x600+ DeleteFileTransactedA
@ stub -version=0x600+ DeleteFileTransactedW
@ stdcall DeleteFileW(wstr)
@ stub -version=0x600+ DeleteProcThreadAttributeList
@ stdcall DeleteTimerQueue(long)
@ stdcall DeleteTimerQueueEx (long long)
@ stdcall DeleteTimerQueueTimer(long long long)
@ stdcall DeleteVolumeMountPointA(str) ;check
@ stdcall DeleteVolumeMountPointW(wstr) ;check
@ stdcall DeviceIoControl(long long ptr long ptr long ptr ptr)
@ stdcall DisableThreadLibraryCalls(long)
@ stub -version=0x600+ DisassociateCurrentThreadFromCallback
@ stdcall DisconnectNamedPipe(long)
@ stdcall DnsHostnameToComputerNameA (str ptr ptr)
@ stdcall DnsHostnameToComputerNameW (wstr ptr ptr)
@ stdcall DosDateTimeToFileTime(long long ptr)
@ stdcall DosPathToSessionPathA(long str str)
@ stdcall DosPathToSessionPathW(long wstr wstr)
@ stdcall DuplicateConsoleHandle(long long long long)
@ stdcall DuplicateHandle(long long long ptr long long long)
@ stdcall EncodePointer(ptr) ntdll.RtlEncodePointer
@ stdcall EncodeSystemPointer(ptr) ntdll.RtlEncodeSystemPointer
@ stdcall EndUpdateResourceA(long long)
@ stdcall EndUpdateResourceW(long long)
@ stdcall EnterCriticalSection(ptr) ntdll.RtlEnterCriticalSection
@ stdcall EnumCalendarInfoA(ptr long long long)
@ stdcall EnumCalendarInfoExA(ptr long long long)
@ stdcall -version=0x600+ EnumCalendarInfoExEx(ptr wstr long wstr long long)
@ stdcall EnumCalendarInfoExW(ptr long long long)
@ stdcall EnumCalendarInfoW(ptr long long long)
@ stdcall EnumDateFormatsA(ptr long long)
@ stdcall EnumDateFormatsExA(ptr long long)
@ stdcall -version=0x600+ EnumDateFormatsExEx(ptr wstr long long)
@ stdcall EnumDateFormatsExW(ptr long long)
@ stdcall EnumDateFormatsW(ptr long long)
@ stdcall EnumLanguageGroupLocalesA(ptr long long ptr)
@ stdcall EnumLanguageGroupLocalesW(ptr long long ptr)
@ stdcall EnumResourceLanguagesA(long str str ptr long)
@ stub -version=0x600+ EnumResourceLanguagesExA
@ stub -version=0x600+ EnumResourceLanguagesExW
@ stdcall EnumResourceLanguagesW(long wstr wstr ptr long)
@ stdcall EnumResourceNamesA(long str ptr long)
@ stub -version=0x600+ EnumResourceNamesExA
@ stub -version=0x600+ EnumResourceNamesExW
@ stdcall EnumResourceNamesW(long wstr ptr long)
@ stdcall EnumResourceTypesA(long ptr long)
@ stub -version=0x600+ EnumResourceTypesExA
@ stub -version=0x600+ EnumResourceTypesExW
@ stdcall EnumResourceTypesW(long ptr long)
@ stdcall EnumSystemCodePagesA(ptr long)
@ stdcall EnumSystemCodePagesW(ptr long)
@ stdcall EnumSystemFirmwareTables(long ptr long)
@ stdcall EnumSystemGeoID(long long ptr)
@ stdcall EnumSystemLanguageGroupsA(ptr long ptr)
@ stdcall EnumSystemLanguageGroupsW(ptr long ptr)
@ stdcall EnumSystemLocalesA(ptr long)
@ stdcall -version=0x600+ EnumSystemLocalesEx(ptr long long ptr)
@ stdcall EnumSystemLocalesW(ptr long)
@ stdcall EnumTimeFormatsA(ptr long long)
@ stdcall -version=0x600+ EnumTimeFormatsEx(ptr wstr long long)
@ stdcall EnumTimeFormatsW(ptr long long)
@ stdcall EnumUILanguagesA(ptr long long)
@ stdcall EnumUILanguagesW(ptr long long)
@ stdcall EnumerateLocalComputerNamesA(ptr long str ptr)
@ stdcall EnumerateLocalComputerNamesW(ptr long wstr ptr)
@ stdcall EraseTape(ptr long long)
@ stdcall EscapeCommFunction(long long)
@ stdcall ExitProcess(long) ; FIXME: ntdll.RtlExitUserProcess
@ stdcall ExitThread(long) ; FIXME: ntdll.RtlExitUserThread
@ stdcall ExitVDM(long long)
@ stdcall ExpandEnvironmentStringsA(str ptr long)
@ stdcall ExpandEnvironmentStringsW(wstr ptr long)
@ stdcall ExpungeConsoleCommandHistoryA(long)
@ stdcall ExpungeConsoleCommandHistoryW(long)
@ stdcall FatalAppExitA(long str)
@ stdcall FatalAppExitW(long wstr)
@ stdcall FatalExit(long)
@ stdcall FileTimeToDosDateTime(ptr ptr ptr)
@ stdcall FileTimeToLocalFileTime(ptr ptr)
@ stdcall FileTimeToSystemTime(ptr ptr)
@ stdcall FillConsoleOutputAttribute(long long long long ptr)
@ stdcall FillConsoleOutputCharacterA(long long long long ptr)
@ stdcall FillConsoleOutputCharacterW(long long long long ptr)
@ stdcall FindActCtxSectionGuid(long ptr long ptr ptr)
@ stdcall FindActCtxSectionStringA(long ptr long str ptr)
@ stdcall FindActCtxSectionStringW(long ptr long wstr ptr)
@ stdcall FindAtomA(str)
@ stdcall FindAtomW(wstr)
@ stdcall FindClose(long)
@ stdcall FindCloseChangeNotification(long)
@ stdcall FindFirstChangeNotificationA(str long long)
@ stdcall FindFirstChangeNotificationW(wstr long long)
@ stdcall FindFirstFileA(str ptr)
@ stdcall FindFirstFileExA(str long ptr long ptr long)
@ stdcall FindFirstFileExW(wstr long ptr long ptr long)
@ stub -version=0x600+ FindFirstFileNameTransactedW
@ stub -version=0x600+ FindFirstFileNameW
@ stub -version=0x600+ FindFirstFileTransactedA
@ stub -version=0x600+ FindFirstFileTransactedW
@ stdcall FindFirstFileW(wstr ptr)
@ stub -version=0x600+ FindFirstStreamTransactedW
@ stdcall FindFirstStreamW(wstr ptr ptr long)
@ stdcall FindFirstVolumeA(ptr long)
@ stdcall FindFirstVolumeMountPointA(str ptr long)
@ stdcall FindFirstVolumeMountPointW(wstr ptr long)
@ stdcall FindFirstVolumeW(ptr long)
@ stub -version=0x600+ FindNLSString
@ stub -version=0x600+ FindNLSStringEx
@ stdcall FindNextChangeNotification(long)
@ stdcall FindNextFileA(long ptr)
@ stub -version=0x600+ FindNextFileNameW
@ stdcall FindNextFileW(long ptr)
@ stdcall FindNextStreamW(ptr ptr)
@ stdcall FindNextVolumeA(long ptr long)
@ stdcall FindNextVolumeMountPointA(long str long)
@ stdcall FindNextVolumeMountPointW(long wstr long)
@ stdcall FindNextVolumeW(long ptr long)
@ stdcall FindResourceA(long str str)
@ stdcall FindResourceExA(long str str long)
@ stdcall FindResourceExW(long wstr wstr long)
@ stdcall FindResourceW(long wstr wstr)
@ stdcall FindVolumeClose(ptr)
@ stdcall FindVolumeMountPointClose(ptr)
@ stdcall FlsAlloc(ptr)
@ stdcall FlsFree(long)
@ stdcall FlsGetValue(long)
@ stdcall FlsSetValue(long ptr)
@ stdcall FlushConsoleInputBuffer(long)
@ stdcall FlushFileBuffers(long)
@ stdcall FlushInstructionCache(long long long)
@ stub -version=0x600+ FlushProcessWriteBuffers
@ stdcall FlushViewOfFile(ptr long)
@ stdcall FoldStringA(long str long ptr long)
@ stdcall FoldStringW(long wstr long ptr long)
@ stdcall FormatMessageA(long ptr long long ptr long ptr)
@ stdcall FormatMessageW(long ptr long long ptr long ptr)
@ stdcall FreeConsole()
@ stdcall FreeEnvironmentStringsA(ptr)
@ stdcall FreeEnvironmentStringsW(ptr)
@ stdcall FreeLibrary(long)
@ stdcall FreeLibraryAndExitThread(long long)
@ stub -version=0x600+ FreeLibraryWhenCallbackReturns
@ stdcall FreeResource(long)
@ stdcall FreeUserPhysicalPages(long long long)
@ stdcall GenerateConsoleCtrlEvent(long long)
@ stdcall GetACP()
@ stdcall -version=0x600+ GetApplicationRecoveryCallback(ptr ptr ptr ptr ptr)
@ stub -version=0x600+ GetApplicationRestartSettings
@ stdcall GetAtomNameA(long ptr long)
@ stdcall GetAtomNameW(long ptr long)
@ stdcall GetBinaryType(str ptr) GetBinaryTypeA
@ stdcall GetBinaryTypeA(str ptr)
@ stdcall GetBinaryTypeW(wstr ptr)
@ stdcall -version=0x501-0x600 GetCPFileNameFromRegistry(long wstr long)
@ stdcall GetCPInfo(long ptr)
@ stdcall GetCPInfoExA(long long ptr)
@ stdcall GetCPInfoExW(long long ptr)
@ stub -version=0x600+ GetCalendarDateFormat
@ stub -version=0x600+ GetCalendarDateFormatEx
@ stub -version=0x600+ GetCalendarDaysInMonth
@ stub -version=0x600+ GetCalendarDifferenceInDays
@ stdcall GetCalendarInfoA(long long long ptr long ptr)
@ stdcall -version=0x600+ GetCalendarInfoEx(wstr long wstr long wstr long ptr)
@ stdcall GetCalendarInfoW(long long long ptr long ptr)
@ stub -version=0x600+ GetCalendarMonthsInYear
@ stub -version=0x600+ GetCalendarSupportedDateRange
@ stub -version=0x600+ GetCalendarWeekNumber
@ stdcall GetComPlusPackageInstallStatus()
@ stdcall GetCommConfig(long ptr long)
@ stdcall GetCommMask(long ptr)
@ stdcall GetCommModemStatus(long ptr)
@ stdcall GetCommProperties(long ptr)
@ stdcall GetCommState(long ptr)
@ stdcall GetCommTimeouts(long ptr)
@ stdcall GetCommandLineA()
@ stdcall GetCommandLineW()
@ stdcall GetCompressedFileSizeA(long ptr)
@ stub -version=0x600+ GetCompressedFileSizeTransactedA
@ stub -version=0x600+ GetCompressedFileSizeTransactedW
@ stdcall GetCompressedFileSizeW(long ptr)
@ stdcall GetComputerNameA(ptr ptr)
@ stdcall GetComputerNameExA(long ptr ptr)
@ stdcall GetComputerNameExW(long ptr ptr)
@ stdcall GetComputerNameW(ptr ptr)
@ stdcall GetConsoleAliasA(str str long str)
@ stdcall GetConsoleAliasExesA(str long)
@ stdcall GetConsoleAliasExesLengthA()
@ stdcall GetConsoleAliasExesLengthW()
@ stdcall GetConsoleAliasExesW(wstr long)
@ stdcall GetConsoleAliasW(wstr ptr long wstr)
@ stdcall GetConsoleAliasesA(str long str)
@ stdcall GetConsoleAliasesLengthA(str)
@ stdcall GetConsoleAliasesLengthW(wstr)
@ stdcall GetConsoleAliasesW(wstr long wstr)
@ stdcall GetConsoleCP()
@ stdcall GetConsoleCharType(long long ptr)
@ stdcall GetConsoleCommandHistoryA(long long long)
@ stdcall GetConsoleCommandHistoryLengthA(long)
@ stdcall GetConsoleCommandHistoryLengthW(long)
@ stdcall GetConsoleCommandHistoryW(long long long)
@ stdcall GetConsoleCursorInfo(long ptr)
@ stdcall GetConsoleCursorMode(long ptr ptr)
@ stdcall GetConsoleDisplayMode(ptr)
@ stdcall GetConsoleFontInfo(long long long ptr)
@ stdcall GetConsoleFontSize(long long)
@ stdcall GetConsoleHardwareState(long long ptr)
@ stub -version=0x600+ GetConsoleHistoryInfo
@ stdcall GetConsoleInputExeNameA(long ptr)
@ stdcall GetConsoleInputExeNameW(long ptr)
@ stdcall GetConsoleInputWaitHandle()
@ stdcall GetConsoleKeyboardLayoutNameA(ptr)
@ stdcall GetConsoleKeyboardLayoutNameW(ptr)
@ stdcall GetConsoleMode(long ptr)
@ stdcall GetConsoleNlsMode(long ptr)
@ stub -version=0x600+ GetConsoleOriginalTitleA
@ stub -version=0x600+ GetConsoleOriginalTitleW
@ stdcall GetConsoleOutputCP()
@ stdcall GetConsoleProcessList(ptr long) ; missing in XP SP3
@ stdcall GetConsoleScreenBufferInfo(long ptr)
@ stub -version=0x600+ GetConsoleScreenBufferInfoEx
@ stdcall GetConsoleSelectionInfo(ptr)
@ stdcall GetConsoleTitleA(ptr long)
@ stdcall GetConsoleTitleW(ptr long)
@ stdcall GetConsoleWindow()
@ stdcall GetCurrencyFormatA(long long str ptr str long)
@ stdcall -version=0x600+ GetCurrencyFormatEx(wstr long wstr ptr wstr long)
@ stdcall GetCurrencyFormatW(long long str ptr str long)
@ stdcall GetCurrentActCtx(ptr)
@ stdcall GetCurrentConsoleFont(long long ptr)
@ stub -version=0x600+ GetCurrentConsoleFontEx
@ stdcall GetCurrentDirectoryA(long ptr)
@ stdcall GetCurrentDirectoryW(long ptr)
@ stdcall -version=0x602+ GetCurrentPackageId(ptr ptr)
@ stdcall -norelay GetCurrentProcess()
@ stdcall -norelay GetCurrentProcessId()
@ stdcall GetCurrentProcessorNumber() ntdll.RtlGetCurrentProcessorNumber
;@ stdcall -version=0x601+ GetCurrentProcessorNumberEx(ptr) ntdll.RtlGetCurrentProcessorNumberEx
@ stdcall -norelay GetCurrentThread()
@ stdcall -norelay GetCurrentThreadId()
@ stdcall GetDateFormatA(long long ptr str ptr long)
@ stdcall -version=0x600+ GetDateFormatEx(wstr long ptr wstr wstr long wstr)
@ stdcall GetDateFormatW(long long ptr wstr ptr long)
@ stdcall GetDefaultCommConfigA(str ptr long)
@ stdcall GetDefaultCommConfigW(wstr ptr long)
@ stdcall -version=0x501-0x502 GetDefaultSortkeySize(ptr)
@ stdcall GetDevicePowerState(long ptr)
@ stdcall GetDiskFreeSpaceA(str ptr ptr ptr ptr)
@ stdcall GetDiskFreeSpaceExA (str ptr ptr ptr)
@ stdcall GetDiskFreeSpaceExW (wstr ptr ptr ptr)
@ stdcall GetDiskFreeSpaceW(wstr ptr ptr ptr ptr)
@ stdcall GetDllDirectoryA(long ptr)
@ stdcall GetDllDirectoryW(long ptr)
@ stdcall GetDriveTypeA(str)
@ stdcall GetDriveTypeW(wstr)
@ stub -version=0x600+ GetDurationFormat
@ stub -version=0x600+ GetDurationFormatEx
@ stub -version=0x600+ GetDynamicTimeZoneInformation
@ stdcall GetEnvironmentStrings()
@ stdcall GetEnvironmentStringsA() GetEnvironmentStrings
@ stdcall GetEnvironmentStringsW()
@ stdcall GetEnvironmentVariableA(str ptr long)
@ stdcall GetEnvironmentVariableW(wstr ptr long)
@ stdcall -version=0x600+ GetErrorMode()
@ stdcall GetExitCodeProcess(long ptr)
@ stdcall GetExitCodeThread(long ptr)
@ stdcall GetExpandedNameA(str ptr)
@ stdcall GetExpandedNameW(wstr ptr)
@ stdcall GetFileAttributesA(str)
@ stdcall GetFileAttributesExA(str long ptr)
@ stdcall GetFileAttributesExW(wstr long ptr)
@ stub -version=0x600+ GetFileAttributesTransactedA
@ stub -version=0x600+ GetFileAttributesTransactedW
@ stdcall GetFileAttributesW(wstr)
@ stdcall -version=0x600+ GetFileBandwidthReservation(ptr ptr ptr ptr ptr ptr)
@ stdcall GetFileInformationByHandle(long ptr)
@ stdcall -version=0x600+ GetFileInformationByHandleEx(ptr long ptr long)
@ stdcall -version=0x600+ GetFileMUIInfo(long wstr ptr ptr)
@ stdcall -version=0x600+ GetFileMUIPath(long wstr wstr ptr wstr ptr ptr)
@ stdcall GetFileSize(long ptr)
@ stdcall GetFileSizeEx(long ptr)
@ stdcall GetFileTime(long ptr ptr ptr)
@ stdcall GetFileType(long)
@ stdcall -version=0x600+ GetFinalPathNameByHandleA(ptr str long long)
@ stdcall -version=0x600+ GetFinalPathNameByHandleW(ptr wstr long long)
@ stdcall GetFirmwareEnvironmentVariableA(str str ptr long)
@ stdcall GetFirmwareEnvironmentVariableW(wstr wstr ptr long)
@ stdcall GetFullPathNameA(str long ptr ptr)
@ stub -version=0x600+ GetFullPathNameTransactedA
@ stub -version=0x600+ GetFullPathNameTransactedW
@ stdcall GetFullPathNameW(wstr long ptr ptr)
@ stdcall GetGeoInfoA(long long ptr long long)
@ stdcall GetGeoInfoW(long long ptr long long)
@ stdcall -i386 GetHandleContext(long) ; missing on x64
@ stdcall GetHandleInformation(long ptr)
@ stdcall GetLargePageMinimum()
@ stdcall GetLargestConsoleWindowSize(long)
@ stdcall GetLastError() ntdll.RtlGetLastWin32Error
@ stdcall -version=0x500-0x502 GetLinguistLangSize(ptr)
@ stdcall GetLocalTime(ptr)
@ stdcall GetLocaleInfoA(long long ptr long)
@ stdcall -version=0x600+ GetLocaleInfoEx(wstr long ptr long)
@ stdcall GetLocaleInfoW(long long ptr long)
@ stdcall -version=0x600+ IsValidLocaleName(wstr)
@ stdcall GetLogicalDriveStringsA(long ptr)
@ stdcall GetLogicalDriveStringsW(long ptr)
@ stdcall GetLogicalDrives()
@ stdcall GetLogicalProcessorInformation(ptr ptr)
@ stdcall GetLongPathNameA (str long long)
@ stub -version=0x600+ GetLongPathNameTransactedA
@ stub -version=0x600+ GetLongPathNameTransactedW
@ stdcall GetLongPathNameW (wstr long long)
@ stdcall GetMailslotInfo(long ptr ptr ptr ptr)
@ stdcall GetModuleFileNameA(long ptr long)
@ stdcall GetModuleFileNameW(long ptr long)
@ stdcall GetModuleHandleA(str)
@ stdcall GetModuleHandleExA(long ptr ptr)
@ stdcall GetModuleHandleExW(long ptr ptr)
@ stdcall GetModuleHandleW(wstr)
@ stdcall GetNLSVersion(long long ptr)
@ stdcall GetNLSVersionEx(long wstr ptr)
@ stub -version=0x600+ GetNamedPipeAttribute
@ stub -version=0x600+ GetNamedPipeClientComputerNameA
@ stub -version=0x600+ GetNamedPipeClientComputerNameW
@ stub -version=0x600+ GetNamedPipeClientProcessId
@ stub -version=0x600+ GetNamedPipeClientSessionId
@ stdcall GetNamedPipeHandleStateA(long ptr ptr ptr ptr str long)
@ stdcall GetNamedPipeHandleStateW(long ptr ptr ptr ptr wstr long)
@ stdcall GetNamedPipeInfo(long ptr ptr ptr ptr)
@ stub -version=0x600+ GetNamedPipeServerProcessId
@ stub -version=0x600+ GetNamedPipeServerSessionId
@ stdcall GetNativeSystemInfo(ptr)
@ stdcall GetNextVDMCommand(long)
@ stdcall -version=0x500-0x502 GetNlsSectionName(long long long str str long)
@ stdcall GetNumaAvailableMemoryNode(long ptr)
@ stdcall -version=0x601+ GetNumaAvailableMemoryNodeEx(long ptr)
@ stdcall GetNumaHighestNodeNumber(ptr)
@ stdcall GetNumaNodeProcessorMask(long ptr)
@ stdcall -version=0x601+ GetNumaNodeNumberFromHandle(ptr ptr)
@ stdcall -version=0x601+ GetNumaNodeProcessorMaskEx(long ptr)
@ stdcall GetNumaProcessorNode(long ptr)
@ stdcall -version=0x601+ GetNumaProcessorNodeEx(ptr ptr)
@ stub -version=0x600+ GetNumaProximityNode	
@ stdcall -version=0x601+ GetNumaProximityNodeEx(long ptr)
@ stdcall GetNumberFormatA(long long str ptr ptr long)
@ stdcall -version=0x600+ GetNumberFormatEx(wstr long wstr ptr wstr long)
@ stdcall GetNumberFormatW(long long wstr ptr ptr long)
@ stdcall GetNumberOfConsoleFonts()
@ stdcall GetNumberOfConsoleInputEvents(long ptr)
@ stdcall GetNumberOfConsoleMouseButtons(ptr)
@ stdcall GetOEMCP()
@ stdcall GetOverlappedResult(long ptr ptr long)
@ stub -version=0x600+ GetPhysicallyInstalledSystemMemory
@ stdcall GetPriorityClass(long)
@ stdcall GetPrivateProfileIntA(str str long str)
@ stdcall GetPrivateProfileIntW(wstr wstr long wstr)
@ stdcall GetPrivateProfileSectionA(str ptr long str)
@ stdcall GetPrivateProfileSectionNamesA(ptr long str)
@ stdcall GetPrivateProfileSectionNamesW(ptr long wstr)
@ stdcall GetPrivateProfileSectionW(wstr ptr long wstr)
@ stdcall GetPrivateProfileStringA(str str str ptr long str)
@ stdcall GetPrivateProfileStringW(wstr wstr wstr ptr long wstr)
@ stdcall GetPrivateProfileStructA (str str ptr long str)
@ stdcall GetPrivateProfileStructW(wstr wstr ptr long wstr)
@ stdcall GetProcAddress(long str)
@ stdcall GetProcessAffinityMask(long ptr ptr)
@ stub -version=0x600+ GetProcessDEPPolicy
@ stdcall GetProcessHandleCount(long ptr)
@ stdcall -norelay GetProcessHeap()
@ stdcall GetProcessHeaps(long ptr)
@ stdcall GetProcessId(long)
@ stdcall GetProcessIdOfThread(ptr)
@ stdcall GetProcessIoCounters(long ptr)
@ stdcall GetProcessPriorityBoost(long ptr)
@ stdcall GetProcessShutdownParameters(ptr ptr)
@ stdcall GetProcessTimes(long ptr ptr ptr ptr)
@ stdcall GetProcessVersion(long)
@ stdcall GetProcessWorkingSetSize(long ptr ptr)
@ stdcall GetProcessWorkingSetSizeEx(long ptr ptr long)
@ stdcall -version=0x600+ GetProductInfo() ntdll.RtlGetProductInfo
@ stdcall GetProfileIntA(str str long)
@ stdcall GetProfileIntW(wstr wstr long)
@ stdcall GetProfileSectionA(str ptr long)
@ stdcall GetProfileSectionW(wstr ptr long)
@ stdcall GetProfileStringA(str str str ptr long)
@ stdcall GetProfileStringW(wstr wstr wstr ptr long)
@ stdcall GetQueuedCompletionStatus(long ptr ptr ptr long)
@ stdcall GetShortPathNameA(str ptr long)
@ stdcall GetShortPathNameW(wstr ptr long)
@ stdcall GetStartupInfoA(ptr)
@ stdcall GetStartupInfoW(ptr)
@ stdcall GetStdHandle(long)
@ stub -version=0x600+ GetStringScripts
@ stdcall GetStringTypeA(long long str long ptr)
@ stdcall GetStringTypeExA(long long str long ptr)
@ stdcall GetStringTypeExW(long long wstr long ptr)
@ stdcall GetStringTypeW(long wstr long ptr)
@ stdcall -version=0x600+ GetSystemDEPPolicy()
@ stdcall GetSystemDefaultLCID()
@ stdcall GetSystemDefaultLangID()
@ stub -version=0x600+ GetSystemDefaultLocaleName
@ stdcall GetSystemDefaultUILanguage()
@ stdcall GetSystemDirectoryA(ptr long)
@ stdcall GetSystemDirectoryW(ptr long)
@ stdcall GetSystemFileCacheSize(ptr ptr ptr)
@ stdcall GetSystemFirmwareTable(long long ptr long)
@ stdcall GetSystemInfo(ptr)
@ stdcall GetSystemPowerStatus(ptr)
@ stdcall -version=0x600+ GetSystemPreferredUILanguages(long ptr wstr ptr)
@ stdcall GetSystemRegistryQuota(ptr ptr)
@ stdcall GetSystemTime(ptr)
@ stdcall GetSystemTimeAdjustment(ptr ptr ptr)
@ stdcall GetSystemTimeAsFileTime(ptr)
@ stdcall -version=0x602+ GetSystemTimePreciseAsFileTime(ptr)
@ stdcall GetSystemTimes(ptr ptr ptr)
@ stdcall GetSystemWindowsDirectoryA(ptr long)
@ stdcall GetSystemWindowsDirectoryW(ptr long)
@ stdcall GetSystemWow64DirectoryA(ptr long)
@ stdcall GetSystemWow64DirectoryW(ptr long)
@ stdcall GetTapeParameters(ptr long ptr ptr)
@ stdcall GetTapePosition(ptr long ptr ptr ptr)
@ stdcall GetTapeStatus(ptr)
@ stdcall GetTempFileNameA(str str long ptr)
@ stdcall GetTempFileNameW(wstr wstr long ptr)
@ stdcall GetTempPathA(long ptr)
@ stdcall GetTempPathW(long ptr)
@ stdcall GetThreadContext(long ptr)
@ stub -version=0x600+ GetThreadErrorMode
@ stdcall GetThreadIOPendingFlag(long ptr)
@ stdcall GetThreadId(ptr)
@ stdcall GetThreadLocale()
@ stdcall -version=0x600+ GetThreadPreferredUILanguages(long ptr wstr ptr)
@ stdcall GetThreadPriority(long)
@ stdcall GetThreadPriorityBoost(long ptr)
@ stdcall GetThreadSelectorEntry(long long ptr)
@ stdcall GetThreadTimes(long ptr ptr ptr ptr)
@ stdcall -version=0x600+ GetThreadUILanguage()
@ stdcall GetTickCount()
@ stdcall -version=0x600+ -ret64 GetTickCount64()
@ stdcall GetTimeFormatA(long long ptr str ptr long)
@ stdcall -version=0x600+ GetTimeFormatEx(wstr long ptr wstr wstr long)
@ stdcall GetTimeFormatW(long long ptr wstr ptr long)
@ stdcall GetTimeZoneInformation(ptr)
@ stub -version=0x600+ GetTimeZoneInformationForYear
@ stdcall -version=0x600+ GetUILanguageInfo(long wstr wstr ptr ptr)
@ stdcall GetUserDefaultLCID()
@ stdcall GetUserDefaultLangID()
@ stdcall -version=0x600+ GetUserDefaultLocaleName(wstr long)
@ stdcall GetUserDefaultUILanguage()
@ stdcall GetUserGeoID(long)
@ stdcall -version=0x600+ GetUserPreferredUILanguages(long ptr wstr ptr)
@ stdcall GetVDMCurrentDirectories(long long)
@ stdcall GetVersion()
@ stdcall GetVersionExA(ptr)
@ stdcall GetVersionExW(ptr)
@ stdcall GetVolumeInformationA(str ptr long ptr ptr ptr ptr long)
@ stub -version=0x600+ GetVolumeInformationByHandleW
@ stdcall GetVolumeInformationW(wstr ptr long ptr ptr ptr ptr long)
@ stdcall GetVolumeNameForVolumeMountPointA(str ptr long)
@ stdcall GetVolumeNameForVolumeMountPointW(wstr ptr long)
@ stdcall GetVolumePathNameA(str ptr long)
@ stdcall GetVolumePathNameW(wstr ptr long)
@ stdcall GetVolumePathNamesForVolumeNameA(str str long ptr)
@ stdcall GetVolumePathNamesForVolumeNameW(wstr wstr long ptr)
@ stdcall GetWindowsDirectoryA(ptr long)
@ stdcall GetWindowsDirectoryW(ptr long)
@ stdcall GetWriteWatch(long ptr long ptr ptr ptr)
@ stdcall GlobalAddAtomA(str)
@ stdcall GlobalAddAtomW(wstr)
@ stdcall GlobalAlloc(long long)
@ stdcall GlobalCompact(long)
@ stdcall GlobalDeleteAtom(long)
@ stdcall GlobalFindAtomA(str)
@ stdcall GlobalFindAtomW(wstr)
@ stdcall GlobalFix(long)
@ stdcall GlobalFlags(long)
@ stdcall GlobalFree(long)
@ stdcall GlobalGetAtomNameA(long ptr long)
@ stdcall GlobalGetAtomNameW(long ptr long)
@ stdcall GlobalHandle(ptr)
@ stdcall GlobalLock(long)
@ stdcall GlobalMemoryStatus(ptr)
@ stdcall GlobalMemoryStatusEx(ptr)
@ stdcall GlobalReAlloc(long long long)
@ stdcall GlobalSize(long)
@ stdcall GlobalUnWire(long)
@ stdcall GlobalUnfix(long)
@ stdcall GlobalUnlock(long)
@ stdcall GlobalWire(long)
@ stdcall Heap32First(ptr long long)
@ stdcall Heap32ListFirst(long ptr)
@ stdcall Heap32ListNext(long ptr)
@ stdcall Heap32Next(ptr)
@ stdcall HeapAlloc(long long long) ntdll.RtlAllocateHeap
@ stdcall HeapCompact(long long)
@ stdcall HeapCreate(long long long)
@ stdcall -version=0x351-0x502 HeapCreateTagsW(ptr long wstr wstr)
@ stdcall HeapDestroy(long)
@ stdcall -version=0x351-0x502 HeapExtend(long long ptr long)
@ stdcall HeapFree(long long long) ntdll.RtlFreeHeap
@ stdcall HeapLock(long)
@ stdcall HeapQueryInformation(long long ptr long ptr)
@ stdcall -version=0x351-0x502 HeapQueryTagW(long long long long ptr)
@ stdcall HeapReAlloc(long long ptr long) ntdll.RtlReAllocateHeap
@ stdcall HeapSetInformation(ptr long ptr long)
@ stdcall HeapSize(long long ptr) ntdll.RtlSizeHeap
@ stdcall HeapSummary(long long ptr)
@ stdcall HeapUnlock(long)
@ stdcall -version=0x351-0x502 HeapUsage(long long long long ptr)
@ stdcall HeapValidate(long long ptr)
@ stdcall HeapWalk(long ptr)
@ stub -version=0x600+ IdnToAscii
@ stub -version=0x600+ IdnToNameprepUnicode
@ stub -version=0x600+ IdnToUnicode
@ stdcall InitAtomTable(long)
@ stdcall -version=0x600+ InitOnceBeginInitialize(ptr long ptr ptr) kernel32_vista.InitOnceBeginInitialize
@ stdcall -version=0x600+ InitOnceComplete(ptr long ptr) kernel32_vista.InitOnceComplete
@ stdcall -version=0x600+ InitOnceExecuteOnce(ptr ptr ptr ptr) kernel32_vista.InitOnceExecuteOnce
@ stdcall -stub -version=0x600+ InitOnceInitialize(ptr)
@ stdcall -version=0x600+ InitializeConditionVariable(ptr) ntdll.RtlInitializeConditionVariable
@ stdcall InitializeCriticalSection(ptr)
@ stdcall InitializeCriticalSectionAndSpinCount(ptr long)
@ stdcall -version=0x600+ InitializeCriticalSectionEx(ptr long long)
@ stdcall -version=0x600+ InitializeProcThreadAttributeList(ptr long long ptr)
@ stdcall InitializeSListHead(ptr) ntdll.RtlInitializeSListHead
@ stdcall -version=0x600+ InitializeSRWLock(ptr) ntdll.RtlInitializeSRWLock
@ stdcall -arch=i386 -ret64 InterlockedCompareExchange64(ptr double double) ntdll.RtlInterlockedCompareExchange64
@ stdcall -arch=i386 InterlockedCompareExchange (ptr long long)
@ stdcall -arch=i386 InterlockedDecrement(ptr)
@ stdcall -arch=i386 InterlockedExchange(ptr long)
@ stdcall -arch=i386 InterlockedExchangeAdd(ptr long)
@ stdcall InterlockedFlushSList(ptr) ntdll.RtlInterlockedFlushSList
@ stdcall -arch=i386 InterlockedIncrement(ptr)
@ stdcall InterlockedPopEntrySList(ptr) ntdll.RtlInterlockedPopEntrySList
@ stdcall InterlockedPushEntrySList(ptr ptr) ntdll.RtlInterlockedPushEntrySList
@ fastcall -version=0x600+ InterlockedPushListSList(ptr ptr ptr long) ntdll.RtlInterlockedPushListSList
@ stdcall InvalidateConsoleDIBits(long long)
@ stdcall IsBadCodePtr(ptr)
@ stdcall -version=0x600+ TryAcquireSRWLockExclusive(ptr) ntdll_vista.RtlTryAcquireSRWLockExclusive
@ stdcall -version=0x600+ TryAcquireSRWLockShared(ptr) ntdll_vista.RtlTryAcquireSRWLockShared
@ stdcall IsBadHugeReadPtr(ptr long)
@ stdcall IsBadHugeWritePtr(ptr long)
@ stdcall IsBadReadPtr(ptr long)
@ stdcall IsBadStringPtrA(ptr long)
@ stdcall IsBadStringPtrW(ptr long)
@ stdcall IsBadWritePtr(ptr long)
@ stub -version=0x600+ IsCalendarLeapDay
@ stub -version=0x600+ IsCalendarLeapMonth
@ stub -version=0x600+ IsCalendarLeapYear
@ stdcall IsDBCSLeadByte(long)
@ stdcall IsDBCSLeadByteEx(long long)
@ stdcall IsDebuggerPresent()
@ stdcall IsNLSDefinedString(long long ptr long long)
@ stub -version=0x600+ IsNormalizedString
@ stdcall IsProcessInJob(long long ptr)
@ stdcall IsProcessorFeaturePresent(long)
@ stdcall IsSystemResumeAutomatic()
@ stdcall -version=0x600+ IsThreadAFiber()
@ stub -version=0x600+ IsThreadpoolTimerSet
@ stdcall IsTimeZoneRedirectionEnabled()
@ stub -version=0x600+ IsValidCalDateTime
@ stdcall IsValidCodePage(long)
@ stdcall IsValidLanguageGroup(long long)
@ stdcall IsValidLocale(long long)
@ stdcall -version=0x501-0x502 IsValidUILanguage(long)
@ stdcall IsWow64Process(ptr ptr)
@ stdcall -version=0x600+ LCIDToLocaleName(long wstr long long)
@ stdcall LCMapStringA(long long str long ptr long)
@ stdcall -version=0x600+ LCMapStringEx(long long wstr long ptr long ptr ptr long)
@ stdcall LCMapStringW(long long wstr long ptr long)
@ stdcall LZClose(long)
@ stdcall LZCloseFile(long)
@ stdcall LZCopy(long long)
@ stdcall LZCreateFileW(ptr long long long ptr)
@ stdcall LZDone()
@ stdcall LZInit(long)
@ stdcall LZOpenFileA(str ptr long)
@ stdcall LZOpenFileW(wstr ptr long)
@ stdcall LZRead(long str long)
@ stdcall LZSeek(long long long)
@ stdcall LZStart()
@ stdcall LeaveCriticalSection(ptr) ntdll.RtlLeaveCriticalSection
@ stdcall -stub -version=0x600+ LeaveCriticalSectionWhenCallbackReturns(ptr ptr)
@ stdcall -stub -version=0x601+ LoadAppInitDlls()
@ stdcall LoadLibraryA(str)
@ stdcall LoadLibraryExA( str long long)
@ stdcall LoadLibraryExW(wstr long long)
@ stdcall LoadLibraryW(wstr)
@ stdcall LoadModule(str ptr)
@ stdcall LoadResource(long long)
@ stdcall -stub -version=0x600+ LoadStringBaseExW()
@ stdcall -stub -version=0x600+ LoadStringBaseW()
@ stdcall LocalAlloc(long long)
@ stdcall LocalCompact(long)
@ stdcall LocalFileTimeToFileTime(ptr ptr)
@ stdcall LocalFlags(long)
@ stdcall LocalFree(long)
@ stdcall LocalHandle(ptr)
@ stdcall LocalLock(long)
@ stdcall LocalReAlloc(long long long)
@ stdcall LocalShrink(long long)
@ stdcall LocalSize(long)
@ stdcall LocalUnlock(long)
@ stdcall -version=0x600+ LocaleNameToLCID(wstr long)
@ stdcall LockFile(long long long long long)
@ stdcall LockFileEx(long long long long long ptr)
@ stdcall LockResource(long)
@ stdcall MapUserPhysicalPages(ptr long ptr)
@ stdcall MapUserPhysicalPagesScatter(ptr long ptr)
@ stdcall MapViewOfFile(long long long long long)
@ stdcall MapViewOfFileEx(long long long long long ptr)
@ stub -version=0x600+ MapViewOfFileExNuma
@ stdcall Module32First(long ptr)
@ stdcall Module32FirstW(long ptr)
@ stdcall Module32Next(long ptr)
@ stdcall Module32NextW(long ptr)
@ stdcall MoveFileA(str str)
@ stdcall MoveFileExA(str str long)
@ stdcall MoveFileExW(wstr wstr long)
@ stub -version=0x600+ MoveFileTransactedA
@ stub -version=0x600+ MoveFileTransactedW
@ stdcall MoveFileW(wstr wstr)
@ stdcall MoveFileWithProgressA(str str ptr ptr long)
@ stdcall MoveFileWithProgressW(wstr wstr ptr ptr long)
@ stdcall MulDiv(long long long)
@ stdcall MultiByteToWideChar(long long str long ptr long)
@ stdcall NeedCurrentDirectoryForExePathA(str)
@ stdcall NeedCurrentDirectoryForExePathW(wstr)
@ stub -version=0x600+ NlsCheckPolicy
@ stdcall -version=0x500-0x600 NlsConvertIntegerToString(long long long wstr long)
@ stub -version=0x600+ NlsEventDataDescCreate
@ stdcall NlsGetCacheUpdateCount()
@ stdcall -version=0x500-0x502 NlsResetProcessLocale()
@ stub -version=0x600+ NlsUpdateLocale
@ stub -version=0x600+ NlsUpdateSystemLocale
@ stub -version=0x600+ NlsWriteEtwEvent
@ stub -version=0x600+ NormalizeString
@ stub -version=0x600+ NotifyUILanguageChange
@ stdcall OpenConsoleW(wstr long long long)
@ stdcall -version=0x500-0x502 OpenDataFile(long long)
@ stdcall OpenEventA(long long str)
@ stdcall OpenEventW(long long wstr)
@ stdcall OpenFile(str ptr long)
@ stdcall -version=0x600+ OpenFileById(ptr ptr long long ptr long)
@ stdcall OpenFileMappingA(long long str)
@ stdcall OpenFileMappingW(long long wstr)
@ stdcall OpenJobObjectA(long long str)
@ stdcall OpenJobObjectW(long long wstr)
@ stdcall OpenMutexA(long long str)
@ stdcall OpenMutexW(long long wstr)
@ stub -version=0x600+ OpenPrivateNamespaceA
@ stub -version=0x600+ OpenPrivateNamespaceW
@ stdcall OpenProcess(long long long)
@ stdcall OpenProfileUserMapping()
@ stdcall OpenSemaphoreA(long long str)
@ stdcall OpenSemaphoreW(long long wstr)
@ stdcall OpenThread(long long long)
@ stdcall OpenWaitableTimerA(long long str)
@ stdcall OpenWaitableTimerW(long long wstr)
@ stdcall OutputDebugStringA(str)
@ stdcall OutputDebugStringW(wstr)
@ stdcall PeekConsoleInputA(ptr ptr long ptr)
@ stdcall PeekConsoleInputW(ptr ptr long ptr)
@ stdcall PeekNamedPipe(long ptr long ptr ptr ptr)
@ stdcall PostQueuedCompletionStatus(long long ptr ptr)
@ stdcall PrepareTape(ptr long long)
@ stdcall PrivCopyFileExW(wstr wstr ptr ptr long long)
@ stdcall PrivMoveFileIdentityW(long long long)
@ stdcall Process32First (ptr ptr)
@ stdcall Process32FirstW (ptr ptr)
@ stdcall Process32Next (ptr ptr)
@ stdcall Process32NextW (ptr ptr)
@ stdcall ProcessIdToSessionId(long ptr)
@ stdcall PulseEvent(long)
@ stdcall PurgeComm(long long)
@ stub -version=0x600+ QueryActCtxSettingsW
@ stdcall QueryActCtxW(long ptr ptr long ptr long ptr)
@ stdcall QueryDepthSList(ptr) ntdll.RtlQueryDepthSList
@ stdcall QueryDosDeviceA(str ptr long)
@ stdcall QueryDosDeviceW(wstr ptr long)
@ stdcall -version=0x600+ QueryFullProcessImageNameA(ptr long str ptr)
@ stdcall -version=0x600+ QueryFullProcessImageNameW(ptr long wstr ptr)
@ stub -version=0x600+ QueryIdleProcessorCycleTime
@ stdcall QueryInformationJobObject(long long ptr long ptr)
@ stdcall QueryMemoryResourceNotification(ptr ptr)
@ stdcall QueryPerformanceCounter(ptr)
@ stdcall QueryPerformanceFrequency(ptr)
@ stub -version=0x600+ QueryProcessAffinityUpdateMode
@ stub -version=0x600+ QueryProcessCycleTime
@ stub -version=0x600+ QueryThreadCycleTime
@ stdcall QueueUserAPC(ptr long long)
@ stdcall QueueUserWorkItem(ptr ptr long)
@ stdcall -norelay RaiseException(long long long ptr)
@ stdcall -stub RaiseFailFastException(ptr ptr long)
@ stdcall ReOpenFile(ptr long long long)
@ stdcall ReadConsoleA(long ptr long ptr ptr)
@ stdcall ReadConsoleInputA(long ptr long ptr)
@ stdcall ReadConsoleInputExA(long ptr long ptr long)
@ stdcall ReadConsoleInputExW(long ptr long ptr long)
@ stdcall ReadConsoleInputW(long ptr long ptr)
@ stdcall ReadConsoleOutputA(long ptr long long ptr)
@ stdcall ReadConsoleOutputAttribute(long ptr long long ptr)
@ stdcall ReadConsoleOutputCharacterA(long ptr long long ptr)
@ stdcall ReadConsoleOutputCharacterW(long ptr long long ptr)
@ stdcall ReadConsoleOutputW(long ptr long long ptr)
@ stdcall ReadConsoleW(long ptr long ptr ptr)
@ stdcall ReadDirectoryChangesW(long ptr long long long ptr ptr ptr)
@ stdcall ReadFile(long ptr long ptr ptr)
@ stdcall ReadFileEx(long ptr long ptr ptr)
@ stdcall ReadFileScatter(long ptr long ptr ptr)
@ stdcall ReadProcessMemory(long ptr ptr long ptr)
@ stdcall -version=0x600+ RegCloseKey(long)
@ stdcall -version=0x600+ RegCreateKeyExA(long str long ptr long long ptr ptr ptr)
@ stdcall -version=0x600+ RegCreateKeyExW(long wstr long ptr long long ptr ptr ptr)
@ stdcall -version=0x600+ RegDeleteKeyExA(long str long long)
@ stdcall -version=0x600+ RegDeleteKeyExW(long wstr long long)
@ stdcall -version=0x600+ RegDeleteKeyValueA(ptr str str)
@ stdcall -version=0x600+ RegDeleteKeyValueW(ptr wstr wstr)
@ stdcall -version=0x600+ RegDeleteTreeA(ptr str)
@ stdcall -version=0x600+ RegDeleteTreeW(ptr wstr)
@ stdcall -version=0x600+ RegDeleteValueA(long str)
@ stdcall -version=0x600+ RegDeleteValueW(long wstr)
@ stdcall -version=0x600+ RegDisablePredefinedCacheEx()
@ stdcall -version=0x600+ RegEnumKeyExA(long long ptr ptr ptr ptr ptr ptr)
@ stdcall -version=0x600+ RegEnumKeyExW(long long ptr ptr ptr ptr ptr ptr)
@ stdcall -version=0x600+ RegEnumValueA(long long ptr ptr ptr ptr ptr ptr)
@ stdcall -version=0x600+ RegEnumValueW(long long ptr ptr ptr ptr ptr ptr)
@ stdcall -version=0x600+ RegFlushKey(long)
@ stdcall -version=0x600+ RegGetKeySecurity(long long ptr ptr)
@ stdcall -version=0x600+ RegGetValueA(long str str long ptr ptr ptr)
@ stdcall -version=0x600+ RegGetValueW(long wstr wstr long ptr ptr ptr)
@ stdcall -version=0x600+ RegisterApplicationRecoveryCallback(ptr ptr long long) kernel32_vista.RegisterApplicationRecoveryCallback
@ stdcall -version=0x600+ RegisterApplicationRestart(wstr long) kernel32_vista.RegisterApplicationRestart
@ stdcall RegisterConsoleIME(ptr ptr)
@ stdcall RegisterConsoleOS2(long)
@ stdcall RegisterConsoleVDM(long long long long long long long long long long long)
@ stdcall RegisterWaitForInputIdle(ptr)
@ stdcall RegisterWaitForSingleObject(ptr long ptr ptr long long)
@ stdcall RegisterWaitForSingleObjectEx(long ptr ptr long long)
@ stdcall RegisterWowBaseHandlers(long)
@ stdcall RegisterWowExec(long)
@ stdcall -version=0x600+ RegLoadKeyA(long str str)
@ stdcall -version=0x600+ RegLoadKeyW(long wstr wstr)
@ stdcall -stub -version=0x600+ RegLoadMUIStringA(long str str long ptr long str)
@ stdcall -stub -version=0x600+ RegLoadMUIStringW(long wstr wstr long ptr long wstr)
@ stdcall -version=0x600+ RegNotifyChangeKeyValue(long long long long long)
@ stdcall -version=0x600+ RegOpenCurrentUser(long ptr)
@ stdcall -version=0x600+ RegOpenKeyExA(long str long long ptr)
@ stdcall -version=0x600+ RegOpenKeyExW(long wstr long long ptr)
@ stdcall -version=0x600+ RegOpenUserClassesRoot(ptr long long ptr)
@ stdcall -version=0x600+ RegQueryInfoKeyA(long ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr)
@ stdcall -version=0x600+ RegQueryInfoKeyW(long ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr)
@ stdcall -version=0x600+ RegQueryValueExA(long str ptr ptr ptr ptr)
@ stdcall -version=0x600+ RegQueryValueExW(long wstr ptr ptr ptr ptr)
@ stdcall -version=0x600+ RegRestoreKeyA(long str long)
@ stdcall -version=0x600+ RegRestoreKeyW(long wstr long)
@ stdcall -version=0x600+ RegSaveKeyExA(long str ptr long)
@ stdcall -version=0x600+ RegSaveKeyExW(long str ptr long)
@ stdcall -version=0x600+ RegSetKeySecurity(long long ptr)
@ stdcall -version=0x600+ RegSetValueExA(long str long long ptr long)
@ stdcall -version=0x600+ RegSetValueExW(long wstr long long ptr long)
@ stdcall -version=0x600+ RegUnLoadKeyA(long str)
@ stdcall -version=0x600+ RegUnLoadKeyW(long wstr)
@ stdcall ReleaseActCtx(ptr)
@ stdcall ReleaseMutex(long)
@ stub -version=0x600+ ReleaseMutexWhenCallbackReturns
@ stdcall -version=0x600+ ReleaseSRWLockExclusive(ptr) ntdll.RtlReleaseSRWLockExclusive
@ stdcall -version=0x600+ ReleaseSRWLockShared(ptr) ntdll.RtlReleaseSRWLockShared
@ stdcall ReleaseSemaphore(long long ptr)
@ stub -version=0x600+ ReleaseSemaphoreWhenCallbackReturns
@ stdcall RemoveDirectoryA(str)
@ stub -version=0x600+ RemoveDirectoryTransactedA
@ stub -version=0x600+ RemoveDirectoryTransactedW
@ stdcall RemoveDirectoryW(wstr)
@ stdcall RemoveLocalAlternateComputerNameA(str long)
@ stdcall RemoveLocalAlternateComputerNameW(wstr long)
@ stub -version=0x600+ RemoveSecureMemoryCacheCallback
@ stdcall RemoveVectoredContinueHandler(ptr) ntdll.RtlRemoveVectoredContinueHandler
@ stdcall RemoveVectoredExceptionHandler(ptr) ntdll.RtlRemoveVectoredExceptionHandler
@ stdcall ReplaceFile(wstr wstr wstr long ptr ptr) ReplaceFileW
@ stdcall ReplaceFileA(str str str long ptr ptr)
@ stdcall ReplaceFileW(wstr wstr wstr long ptr ptr)
@ stub -version=0x600+ ReplacePartitionUnit
@ stdcall RequestDeviceWakeup(long)
@ stdcall RequestWakeupLatency(long)
@ stdcall ResetEvent(long)
@ stdcall ResetWriteWatch(ptr long)
@ stdcall -stub -version=0x600+ ResolveDelayLoadedAPI(ptr ptr ptr ptr ptr long)
@ stdcall RestoreLastError(long) ntdll.RtlRestoreLastWin32Error
@ stdcall ResumeThread(long)
@ stdcall -arch=x86_64 RtlAddFunctionTable(ptr long long) ntdll.RtlAddFunctionTable
@ stdcall -register RtlCaptureContext(ptr) ntdll.RtlCaptureContext
@ stdcall RtlCaptureStackBackTrace(long long ptr ptr) ntdll.RtlCaptureStackBackTrace
@ stdcall -arch=x86_64 RtlCompareMemory(ptr ptr ptr) ntdll.RtlCompareMemory
@ stdcall -arch=x86_64 RtlCopyMemory(ptr ptr ptr) ntdll.memcpy
@ stdcall -arch=x86_64 RtlDeleteFunctionTable(ptr) ntdll.RtlDeleteFunctionTable
@ stdcall RtlFillMemory(ptr long long) ntdll.RtlFillMemory
@ stdcall -arch=x86_64 RtlInstallFunctionTableCallback(double double long ptr ptr ptr) ntdll.RtlInstallFunctionTableCallback
@ stdcall -arch=x86_64 RtlLookupFunctionEntry(ptr ptr ptr) ntdll.RtlLookupFunctionEntry
@ stdcall RtlMoveMemory(ptr ptr long) ntdll.RtlMoveMemory
@ stdcall -arch=x86_64 RtlPcToFileHeader(ptr ptr) ntdll.RtlPcToFileHeader
@ stdcall -arch=x86_64 RtlRaiseException(ptr) ntdll.RtlRaiseException
@ stdcall -arch=x86_64 RtlRestoreContext(ptr ptr) ntdll.RtlRestoreContext
@ stdcall RtlUnwind(ptr ptr ptr ptr) ntdll.RtlUnwind
@ stdcall -arch=x86_64 RtlUnwindEx(ptr ptr ptr ptr ptr ptr) ntdll.RtlUnwindEx
@ stdcall -arch=x86_64 RtlVirtualUnwind(ptr ptr ptr long) ntdll.RtlVirtualUnwind
@ stdcall RtlZeroMemory(ptr long) ntdll.RtlZeroMemory
@ stdcall ScrollConsoleScreenBufferA(long ptr ptr ptr ptr)
@ stdcall ScrollConsoleScreenBufferW(long ptr ptr ptr ptr)
@ stdcall SearchPathA(str str str long ptr ptr)
@ stdcall SearchPathW(wstr wstr wstr long ptr ptr)
@ stdcall -version=0x500-0x502 SetCPGlobal(long)
@ stdcall SetCalendarInfoA(long long long str)
@ stdcall SetCalendarInfoW(long long long wstr)
@ stdcall SetClientTimeZoneInformation(ptr)
@ stdcall SetComPlusPackageInstallStatus(ptr)
@ stdcall SetCommBreak(long)
@ stdcall SetCommConfig(long ptr long)
@ stdcall SetCommMask(long ptr)
@ stdcall SetCommState(long ptr)
@ stdcall SetCommTimeouts(long ptr)
@ stdcall SetComputerNameA(str)
@ stdcall SetComputerNameExA(long str)
@ stdcall SetComputerNameExW(long wstr)
@ stdcall SetComputerNameW(wstr)
@ stdcall SetConsoleActiveScreenBuffer(long)
@ stdcall SetConsoleCP(long)
@ stdcall -version=0x351-0x502 SetConsoleCommandHistoryMode(long)
@ stdcall SetConsoleCtrlHandler(ptr long)
@ stdcall SetConsoleCursor(long long)
@ stdcall SetConsoleCursorInfo(long ptr)
@ stdcall SetConsoleCursorMode(long long long)
@ stdcall SetConsoleCursorPosition(long long)
@ stdcall SetConsoleDisplayMode(long long ptr)
@ stdcall SetConsoleFont(long long)
@ stdcall SetConsoleHardwareState(long long long)
@ stub -version=0x600+ SetConsoleHistoryInfo
@ stdcall SetConsoleIcon(ptr)
@ stdcall SetConsoleInputExeNameA(ptr)
@ stdcall SetConsoleInputExeNameW(ptr)
@ stdcall SetConsoleKeyShortcuts(long long long long)
@ stdcall SetConsoleLocalEUDC(long long long long)
@ stdcall SetConsoleMaximumWindowSize(long long)
@ stdcall SetConsoleMenuClose(long)
@ stdcall SetConsoleMode(long long)
@ stdcall SetConsoleNlsMode(long long)
@ stdcall SetConsoleNumberOfCommandsA(long long)
@ stdcall SetConsoleNumberOfCommandsW(long long)
@ stdcall SetConsoleOS2OemFormat(long)
@ stdcall SetConsoleOutputCP(long)
@ stdcall SetConsolePalette(long long long)
@ stub -version=0x600+ SetConsoleScreenBufferInfoEx
@ stdcall SetConsoleScreenBufferSize(long long)
@ stdcall SetConsoleTextAttribute(long long)
@ stdcall SetConsoleTitleA(str)
@ stdcall SetConsoleTitleW(wstr)
@ stdcall SetConsoleWindowInfo(long long ptr)
@ stdcall SetCriticalSectionSpinCount(ptr long) ntdll.RtlSetCriticalSectionSpinCount
@ stub -version=0x600+ SetCurrentConsoleFontEx
@ stdcall SetCurrentDirectoryA(str)
@ stdcall SetCurrentDirectoryW(wstr)
@ stdcall SetDefaultCommConfigA(str ptr long)
@ stdcall SetDefaultCommConfigW(wstr ptr long)
@ stdcall -version=0x600+ SetDefaultDllDirectories(long)
@ stdcall SetDllDirectoryA(str)
@ stdcall SetDllDirectoryW(wstr)
@ stub -version=0x600+ SetDynamicTimeZoneInformation
@ stdcall SetEndOfFile(long)
@ stdcall SetEnvironmentStringsA(ptr)
@ stdcall SetEnvironmentStringsW(ptr)
@ stdcall SetEnvironmentVariableA(str str)
@ stdcall SetEnvironmentVariableW(wstr wstr)
@ stdcall SetErrorMode(long)
@ stdcall SetEvent(long)
@ stub -version=0x600+ SetEventWhenCallbackReturns
@ stdcall SetFileApisToANSI()
@ stdcall SetFileApisToOEM()
@ stdcall SetFileAttributesA(str long)
@ stub -version=0x600+ SetFileAttributesTransactedA
@ stub -version=0x600+ SetFileAttributesTransactedW
@ stdcall SetFileAttributesW(wstr long)
@ stdcall -version=0x600+ SetFileBandwidthReservation(ptr long long long ptr ptr)
@ stdcall SetFileCompletionNotificationModes(ptr long)
@ stub -version=0x600+ SetFileInformationByHandle
@ stub -version=0x600+ SetFileIoOverlappedRange
@ stdcall SetFilePointer(long long ptr long)
@ stdcall SetFilePointerEx(long double ptr long)
@ stdcall SetFileShortNameA(long str)
@ stdcall SetFileShortNameW(long wstr)
@ stdcall SetFileTime(long ptr ptr ptr)
@ stdcall SetFileValidData(long double)
@ stdcall SetFirmwareEnvironmentVariableA(str str ptr long)
@ stdcall SetFirmwareEnvironmentVariableW(wstr wstr ptr long)
@ stdcall -i386 SetHandleContext(long long)
@ stdcall SetHandleCount(long)
@ stdcall SetHandleInformation(long long long)
@ stdcall SetInformationJobObject(long long ptr long)
@ stdcall SetLastConsoleEventActive() ; missing in XP SP3
@ stdcall SetLastError(long) ntdll.RtlSetLastWin32Error
@ stdcall SetLocalPrimaryComputerNameA(long long) ; missing in XP SP3
@ stdcall SetLocalPrimaryComputerNameW(long long) ; missing in XP SP3
@ stdcall SetLocalTime(ptr)
@ stdcall SetLocaleInfoA(long long str)
@ stdcall SetLocaleInfoW(long long wstr)
@ stdcall SetMailslotInfo(long long)
@ stdcall SetMessageWaitingIndicator(ptr long)
@ stub -version=0x600+ SetNamedPipeAttribute
@ stdcall SetNamedPipeHandleState(long ptr ptr ptr)
@ stdcall SetPriorityClass(long long)
@ stdcall SetProcessAffinityMask(long long)
@ stdcall -stub -version=0x600+ SetProcessAffinityUpdateMode(ptr long)
@ stdcall -version=0x600+ SetProcessDEPPolicy(long)
@ stdcall SetProcessPriorityBoost(long long)
@ stdcall SetProcessShutdownParameters(long long)
@ stdcall SetProcessWorkingSetSize(long long long)
@ stdcall SetProcessWorkingSetSizeEx(long long long long)
@ stdcall  -version=0x601+ SetSearchPathMode(long)
@ stdcall SetStdHandle(long long)
@ stub -version=0x600+ SetStdHandleEx
@ stdcall SetSystemFileCacheSize(long long long)
@ stdcall SetSystemPowerState(long long)
@ stdcall SetSystemTime(ptr)
@ stdcall SetSystemTimeAdjustment(long long)
@ stdcall SetTapeParameters(ptr long ptr)
@ stdcall SetTapePosition(ptr long long long long long)
@ stdcall SetTermsrvAppInstallMode(long)
@ stdcall SetThreadAffinityMask(long long)
@ stdcall SetThreadContext(long ptr)
@ stdcall -stub -version=0x601+ SetThreadErrorMode(long long)
@ stdcall SetThreadExecutionState(long)
@ stdcall SetThreadIdealProcessor(long long)
@ stdcall SetThreadLocale(long)
@ stdcall -version=0x600+ SetThreadPreferredUILanguages(long wstr ptr)
@ stdcall SetThreadPriority(long long)
@ stdcall SetThreadPriorityBoost(long long)
@ stdcall SetThreadStackGuarantee(ptr)
@ stdcall SetThreadUILanguage(long)
@ stdcall -stub -version=0x600+ SetThreadpoolThreadMaximum(ptr long)
@ stdcall -stub -version=0x600+ SetThreadpoolThreadMinimum(ptr long)
@ stdcall -stub -version=0x600+ SetThreadpoolTimer(ptr ptr long long)
@ stdcall -stub -version=0x600+ SetThreadpoolWait(ptr ptr ptr)
@ stdcall SetTimeZoneInformation(ptr)
@ stdcall SetTimerQueueTimer(long ptr ptr long long long)
@ stdcall SetUnhandledExceptionFilter(ptr)
@ stdcall SetUserGeoID(long)
@ stdcall SetVDMCurrentDirectories(long long)
@ stdcall SetVolumeLabelA(str str)
@ stdcall SetVolumeLabelW(wstr wstr)
@ stdcall SetVolumeMountPointA(str str)
@ stdcall SetVolumeMountPointW(wstr wstr)
@ stdcall SetWaitableTimer(long ptr long ptr ptr long)
@ stdcall -version=0x601+ SetWaitableTimerEx(long ptr long ptr ptr ptr long)
@ stdcall SetupComm(long long long)
@ stdcall ShowConsoleCursor(long long)
@ stdcall SignalObjectAndWait(long long long long)
@ stdcall SizeofResource(long long)
@ stdcall Sleep(long)
@ stdcall -version=0x600+ SleepConditionVariableCS(ptr ptr long)
@ stdcall -version=0x600+ SleepConditionVariableSRW(ptr ptr long long)
@ stdcall SleepEx(long long)
@ stub -version=0x600+ StartThreadpoolIo
@ stub -version=0x600+ SubmitThreadpoolWork
@ stdcall SuspendThread(long)
@ stdcall SwitchToFiber(ptr)
@ stdcall SwitchToThread()
@ stdcall SystemTimeToFileTime(ptr ptr)
@ stdcall SystemTimeToTzSpecificLocalTime (ptr ptr ptr)
@ stdcall TerminateJobObject(ptr long)
@ stdcall TerminateProcess(ptr long)
@ stdcall TerminateThread(ptr long)
@ stdcall TermsrvAppInstallMode()
@ stdcall Thread32First(long ptr)
@ stdcall Thread32Next(long ptr)
@ stdcall TlsAlloc()
@ stdcall TlsFree(long)
@ stdcall -norelay TlsGetValue(long)
@ stdcall -norelay TlsSetValue(long ptr)
@ stdcall Toolhelp32ReadProcessMemory(long ptr ptr long ptr)
@ stdcall TransactNamedPipe(long ptr long ptr long ptr ptr)
@ stdcall TransmitCommChar(long long)
@ stdcall TryEnterCriticalSection(ptr) ntdll.RtlTryEnterCriticalSection
;@ stub -version=0x600+ TrySubmitThreadpoolCallback
@ stdcall TzSpecificLocalTimeToSystemTime(ptr ptr ptr)
@ stdcall UTRegister(long str str str ptr ptr ptr)
@ stdcall UTUnRegister(long)
@ stdcall UnhandledExceptionFilter(ptr)
@ stdcall UnlockFile(long long long long long)
@ stdcall UnlockFileEx(long long long long ptr)
@ stdcall UnmapViewOfFile(ptr)
@ stub -version=0x600+ UnregisterApplicationRecoveryCallback
@ stub -version=0x600+ UnregisterApplicationRestart
@ stdcall UnregisterConsoleIME()
@ stdcall UnregisterWait(long)
@ stdcall UnregisterWaitEx(long long)
@ stub -version=0x600+ UpdateCalendarDayOfWeek
@ stub -version=0x600+ UpdateProcThreadAttribute
@ stdcall UpdateResourceA(long str str long ptr long)
@ stdcall UpdateResourceW(long wstr wstr long ptr long)
@ stdcall VDMConsoleOperation(long long)
@ stdcall VDMOperationStarted(long)
@ stdcall -version=0x500-0x502 ValidateLCType(long long ptr ptr)
@ stdcall -version=0x500-0x502 ValidateLocale(long)
@ stdcall VerLanguageNameA(long str long)
@ stdcall VerLanguageNameW(long wstr long)
@ stdcall -ret64 VerSetConditionMask(long long long long) ntdll.VerSetConditionMask
@ stdcall VerifyConsoleIoHandle(long)
@ stub -version=0x600+ VerifyScripts
@ stdcall VerifyVersionInfoA(long long double)
@ stdcall VerifyVersionInfoW(long long double)
@ stdcall VirtualAlloc(ptr long long long)
@ stdcall VirtualAllocEx(long ptr long long long)
@ stub -version=0x600+ VirtualAllocExNuma
@ stdcall VirtualFree(ptr long long)
@ stdcall VirtualFreeEx(long ptr long long)
@ stdcall VirtualLock(ptr long)
@ stdcall VirtualProtect(ptr long long ptr)
@ stdcall VirtualProtectEx(long ptr long long ptr)
@ stdcall VirtualQuery(ptr ptr long)
@ stdcall VirtualQueryEx(long ptr ptr long)
@ stdcall VirtualUnlock(ptr long)
@ stdcall WTSGetActiveConsoleSessionId()
@ stdcall WaitCommEvent(long ptr ptr)
@ stdcall WaitForDebugEvent(ptr long)
@ stdcall WaitForMultipleObjects(long ptr long long)
@ stdcall WaitForMultipleObjectsEx(long ptr long long long)
@ stdcall WaitForSingleObject(long long)
@ stdcall WaitForSingleObjectEx(long long long)
@ stub -version=0x600+ WaitForThreadpoolIoCallbacks
@ stub -version=0x600+ WaitForThreadpoolTimerCallbacks
@ stub -version=0x600+ WaitForThreadpoolWaitCallbacks
@ stub -version=0x600+ WaitForThreadpoolWorkCallbacks
@ stdcall WaitNamedPipeA (str long)
@ stdcall WaitNamedPipeW (wstr long)
@ stdcall -version=0x600+ WakeAllConditionVariable(ptr) kernel32_vista.WakeAllConditionVariable
@ stdcall -version=0x600+ WakeConditionVariable(ptr) kernel32_vista.WakeConditionVariable
@ stdcall -stub -version=0x600+ WerGetFlags(ptr ptr)
@ stdcall -stub -version=0x600+ WerRegisterFile(wstr long long)
@ stdcall -stub -version=0x600+ WerRegisterMemoryBlock(ptr long)
@ stdcall -stub -version=0x600+ WerSetFlags(long)
@ stdcall -stub -version=0x600+ WerUnregisterFile(wstr)
@ stdcall -stub -version=0x600+ WerUnregisterMemoryBlock(ptr)
@ stub -version=0x600+ WerpCleanupMessageMapping
@ stub -version=0x600+ WerpInitiateRemoteRecovery
@ stub -version=0x600+ WerpNotifyLoadStringResource
@ stub -version=0x600+ WerpNotifyLoadStringResourceEx
@ stub -version=0x600+ WerpNotifyUseStringResource
@ stub -version=0x600+ WerpStringLookup
@ stdcall WideCharToMultiByte(long long wstr long ptr long ptr ptr)
@ stdcall WinExec(str long)
@ stdcall Wow64DisableWow64FsRedirection(ptr)
@ stdcall Wow64EnableWow64FsRedirection(long)
@ stub -version=0x600+ Wow64GetThreadContext
@ stdcall Wow64RevertWow64FsRedirection(ptr)
@ stub -version=0x600+ Wow64SetThreadContext
@ stub -version=0x600+ Wow64SuspendThread
@ stdcall WriteConsoleA(long ptr long ptr ptr)
@ stdcall WriteConsoleInputA(long ptr long ptr)
@ stdcall WriteConsoleInputVDMA(long long long long)
@ stdcall WriteConsoleInputVDMW(long long long long)
@ stdcall WriteConsoleInputW(long ptr long ptr)
@ stdcall WriteConsoleOutputA(long ptr long long ptr)
@ stdcall WriteConsoleOutputAttribute(long ptr long long ptr)
@ stdcall WriteConsoleOutputCharacterA(long ptr long long ptr)
@ stdcall WriteConsoleOutputCharacterW(long ptr long long ptr)
@ stdcall WriteConsoleOutputW(long ptr long long ptr)
@ stdcall WriteConsoleW(long ptr long ptr ptr)
@ stdcall WriteFile(long ptr long ptr ptr)
@ stdcall WriteFileEx(long ptr long ptr ptr)
@ stdcall WriteFileGather(long ptr long ptr ptr)
@ stdcall WritePrivateProfileSectionA(str str str)
@ stdcall WritePrivateProfileSectionW(wstr wstr wstr)
@ stdcall WritePrivateProfileStringA(str str str str)
@ stdcall WritePrivateProfileStringW(wstr wstr wstr wstr)
@ stdcall WritePrivateProfileStructA (str str ptr long str)
@ stdcall WritePrivateProfileStructW(wstr wstr ptr long wstr)
@ stdcall WriteProcessMemory(long ptr ptr long ptr)
@ stdcall WriteProfileSectionA(str str)
@ stdcall WriteProfileSectionW(str str)
@ stdcall WriteProfileStringA(str str str)
@ stdcall WriteProfileStringW(wstr wstr wstr)
@ stdcall WriteTapemark(ptr long long long)
@ stdcall ZombifyActCtx(ptr)
@ stdcall -arch=x86_64,arm64 __C_specific_handler() ntdll.__C_specific_handler
@ stdcall -arch=x86_64,arm64 __chkstk() ntdll.__chkstk
;@ stdcall -arch=x86_64 __misaligned_access() ntdll.__misaligned_access
@ stdcall _hread(long ptr long)
@ stdcall _hwrite(long ptr long)
@ stdcall _lclose(long)
@ stdcall _lcreat(str long)
@ stdcall _llseek(long long long)
@ stdcall -arch=x86_64,arm64 _local_unwind() ntdll._local_unwind
@ stdcall _lopen(str long)
@ stdcall _lread(long ptr long) _hread
@ stdcall _lwrite(long ptr long) _hwrite
@ stdcall lstrcat(str str) lstrcatA
@ stdcall lstrcatA(str str)
@ stdcall lstrcatW(wstr wstr)
@ stdcall lstrcmp(str str) lstrcmpA
@ stdcall lstrcmpA(str str)
@ stdcall lstrcmpW(wstr wstr)
@ stdcall lstrcmpi(str str) lstrcmpiA
@ stdcall lstrcmpiA(str str)
@ stdcall lstrcmpiW(wstr wstr)
@ stdcall lstrcpy(ptr str) lstrcpyA
@ stdcall lstrcpyA(ptr str)
@ stdcall lstrcpyW(ptr wstr)
@ stdcall lstrcpyn(ptr str long) lstrcpynA
@ stdcall lstrcpynA(ptr str long)
@ stdcall lstrcpynW(ptr wstr long)
@ stdcall lstrlen(str) lstrlenA
@ stdcall lstrlenA(str)
@ stdcall lstrlenW(wstr)
;@ stdcall -arch=x86_64 uaw_lstrcmpW(wstr wstr)
;@ stdcall -arch=x86_64 uaw_lstrcmpiW(wstr wstr)
;@ stdcall -arch=x86_64 uaw_lstrlenW(wstr)
;@ stdcall -arch=x86_64 uaw_wcschr(wstr long)
;@ stdcall -arch=x86_64 uaw_wcscpy(ptr wstr)
;@ stdcall -arch=x86_64 uaw_wcsicmp(wstr wstr)
;@ stdcall -arch=x86_64 uaw_wcslen(wstr)
;@ stdcall -arch=x86_64 uaw_wcsrchr(wstr long)
;@ stdcall -version=0x600+ K32EmptyWorkingSet(long)
;@ stdcall -version=0x600+ K32EnumDeviceDrivers(ptr long ptr)
@ stdcall -stub -version=0x600+ K32EnumPageFilesA(ptr ptr)
@ stdcall -stub -version=0x600+ K32EnumPageFilesW(ptr ptr)
@ stdcall -version=0x600+ K32EnumProcessModules(long ptr long ptr)
;@ stdcall -stub -version=0x600+ K32EnumProcessModulesEx(long ptr long ptr long)
@ stdcall -stub -version=0x600+ K32EnumProcesses(ptr long ptr)
@ stdcall -stub -version=0x600+ K32GetDeviceDriverBaseNameA(ptr ptr long)
@ stdcall -stub -version=0x600+ K32GetDeviceDriverBaseNameW(ptr ptr long)
@ stdcall -stub -version=0x600+ K32GetDeviceDriverFileNameA(ptr ptr long)
@ stdcall -stub -version=0x600+ K32GetDeviceDriverFileNameW(ptr ptr long)
@ stdcall -stub -version=0x600+ K32GetMappedFileNameW(long ptr ptr long)
@ stdcall -version=0x600+ K32GetModuleBaseNameA(long long ptr long)
@ stdcall -version=0x600+ K32GetModuleBaseNameW(long long ptr long)
@ stdcall -version=0x600+ K32GetModuleFileNameExA(long long ptr long)
@ stdcall -version=0x600+ K32GetModuleFileNameExW(long long ptr long)
@ stdcall -version=0x600+ K32GetModuleInformation(long long ptr long)
@ stdcall -stub -version=0x601+ K32GetPerformanceInfo(ptr long)
@ stdcall -stub -version=0x601+ K32GetProcessImageFileNameW(ptr wstr long)
@ stdcall -version=0x600+ K32GetProcessMemoryInfo(ptr ptr long)
@ stdcall -stub -version=0x600+ K32QueryWorkingSet(ptr ptr long)
@ stdcall -stub -version=0x600+ K32QueryWorkingSetEx(ptr ptr long)
@ stdcall -stub -version=0x600+ PowerClearRequest(long long)
@ stdcall -stub -version=0x600+ PowerCreateRequest(ptr)
@ stdcall -stub -version=0x600+ PowerSetRequest(long long)


; Dont mind me, these are out of order on purpose
;nt6.0
;AcquireSRWLockExclusive	forwarded to NTDLL function RtlAcquireSRWLockExclusive
;AcquireSRWLockShared	forwarded to NTDLL function RtlAcquireSRWLockShared
;AddSIDToBoundaryDescriptor	implemented as jump to API-MS-Win-Core-Namespace-L1-1-0 in 6.2 and higher
;AddSecureMemoryCacheCallback	begins in SP1;
;implemented as wrapper around NTDLL function RtlRegisterSecureMemoryCacheCallback
;AdjustCalendarDate	undocumented until 2009
;AllocateUserPhysicalPagesNuma	implemented as jump to api-ms-win-core-memory-l1-1-2 in 6.3 and higher
;ApplicationRecoveryFinished	 
;ApplicationRecoveryInProgress	 
;BaseGenerateAppCompatData	 
;BaseThreadInitThunk	 
;CallbackMayRunLong	forwarded to NTDLL function TpCallbackMayRunLong before 6.1;
;implemented as wrapper around NTDLL function TpCallbackMayRunLong in 6.1 and higher
;CancelIoEx	implemented as jump to API-MS-Win-Core-IO-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-io-l1-1-1 in 6.2 and higher
;CancelSynchronousIo	implemented as jump to api-ms-win-core-io-l1-1-1 in 6.2 and higher
;CancelThreadpoolIo	forwarded to NTDLL function TpCancelAsyncIoOperation
;CheckElevation	 
;CheckElevationEnabled	 
;CheckForReadOnlyResource	 
;ClosePrivateNamespace	implemented as jump to API-MS-Win-Core-Namespace-L1-1-0 in 6.2 and higher
;CloseThreadpool	forwarded to NTDLL function TpReleasePool
;CloseThreadpoolCleanupGroup	forwarded to NTDLL function TpReleaseCleanupGroup
;CloseThreadpoolCleanupGroupMembers	forwarded to NTDLL function TpReleaseCleanupGroupMembers
;CloseThreadpoolIo	forwarded to NTDLL function TpReleaseIoCompletion
;CloseThreadpoolTimer	forwarded to NTDLL function TpReleaseTimer
;CloseThreadpoolWait	forwarded to NTDLL function TpReleaseWait
;CloseThreadpoolWork	forwarded to NTDLL function TpReleaseWork
;CompareCalendarDates	 
;CompareStringEx	implemented as jump to API-MS-Win-Core-String-L1-1-0 in 6.1 and higher
;CompareStringOrdinal	implemented as jump to API-MS-Win-Core-String-L1-1-0 in 6.1 and higher
;ConvertCalDateTimeToSystemTime	undocumented until 2009
;ConvertNLSDayOfWeekToWin32DayOfWeek	 
;ConvertSystemTimeToCalDateTime	undocumented until 2009
;CopyFileTransactedA	 
;CopyFileTransactedW	 
;CreateBoundaryDescriptorA	 
;CreateBoundaryDescriptorW	implemented as jump to API-MS-Win-Core-Namespace-L1-1-0 in 6.2 and higher
;CreateDirectoryTransactedA	 
;CreateDirectoryTransactedW	 
;CreateEventExA	implemented as jump to API-MS-Win-Core-Synch-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-synch-l1-2-0 in 6.2 and higher
;CreateEventExW	implemented as jump to API-MS-Win-Core-Synch-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-synch-l1-2-0 in 6.2 and higher
;CreateFileMappingNumaA	 
;CreateFileMappingNumaW	implemented as jump to KERNELBASE in 6.1 only;
;implemented as jump to api-ms-win-core-memory-l1-1-1 in 6.2 only;
;implemented as jump to api-ms-win-core-memory-l1-1-2 in 6.3 and higher
;CreateFileTransactedA	 
;CreateFileTransactedW	 
;CreateHardLinkTransactedA	 
;CreateHardLinkTransactedW	 
;CreateMutexExA	implemented as jump to API-MS-Win-Core-Synch-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-synch-l1-2-0 in 6.2 and higher
;CreateMutexExW	implemented as jump to API-MS-Win-Core-Synch-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-synch-l1-2-0 in 6.2 and higher
;CreatePrivateNamespaceA	 
;CreatePrivateNamespaceW	implemented as jump to API-MS-Win-Core-Namespace-L1-1-0 in 6.2 and higher
;CreateSemaphoreExA	 
;CreateSemaphoreExW	implemented as jump to API-MS-Win-Core-Synch-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-synch-l1-2-0 in 6.2 and higher
;CreateSymbolicLinkA	 
;CreateSymbolicLinkTransactedA	 
;CreateSymbolicLinkTransactedW	 
;CreateSymbolicLinkW	implemented as jump to api-ms-win-core-file-l2-1-0 in 6.2 only;
;implemented as jump to api-ms-win-core-file-l2-1-1 in 6.3 and higher
;CreateThreadpool	implemented as wrapper around NTDLL function TpAllocPool
;CreateThreadpoolCleanupGroup	implemented as wrapper around NTDLL function TpAllocCleanupGroup
;CreateThreadpoolIo	 
;CreateThreadpoolTimer	implemented as wrapper around NTDLL function TpAllocTimer
;CreateThreadpoolWait	implemented as wrapper around NTDLL function TpAllocWait
;CreateThreadpoolWork	implemented as wrapper around NTDLL function TpAllocWork
;CreateWaitableTimerExA	 
;CreateWaitableTimerExW	implemented as jump to API-MS-Win-Core-Synch-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-synch-l1-2-0 in 6.2 and higher
;DeleteBoundaryDescriptor	forwarded to NTDLL function RtlDeleteBoundaryDescriptor before 6.2;
;implemented as jump to API-MS-Win-Core-Namespace-L1-1-0 in 6.2 and higher
;DeleteFileTransactedA	 
;DeleteFileTransactedW	 
;DeleteProcThreadAttributeList	forwarded to API-MS-Win-Core-ProcessThreads-L1-1-0 in 6.1 and higher
;DisassociateCurrentThreadFromCallback	forwarded to NTDLL function TpDisassociateCallback
;EnumCalendarInfoExEx	implemented as jump to KERNELBASE in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l2-1-0 in 6.2 and higher
;EnumDateFormatsExEx	implemented as jump to KERNELBASE in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l2-1-0 in 6.2 and higher
;EnumResourceLanguagesExA	implemented as jump to api-ms-win-core-libraryloader-l1-1-1 in 6.2 only;
;implemented as jump to api-ms-win-core-libraryloader-l1-2-0 in 6.3 and higher
;EnumResourceLanguagesExW	implemented as jump to api-ms-win-core-libraryloader-l1-1-1 in 6.2 only;
;implemented as jump to api-ms-win-core-libraryloader-l1-2-0 in 6.3 and higher
;EnumResourceNamesExA	implemented as jump to api-ms-win-core-libraryloader-l1-1-1 in 6.2 only;
;implemented as jump to api-ms-win-core-libraryloader-l1-2-0 in 6.3 and higher
;EnumResourceNamesExW	implemented as jump to api-ms-win-core-libraryloader-l1-1-1 in 6.2 only;
;implemented as jump to api-ms-win-core-libraryloader-l1-2-0 in 6.3 and higher
;EnumResourceTypesExA	implemented as jump to api-ms-win-core-libraryloader-l1-1-1 in 6.2 only;
;implemented as jump to api-ms-win-core-libraryloader-l1-2-0 in 6.3 and higher
;EnumResourceTypesExW	implemented as jump to api-ms-win-core-libraryloader-l1-1-1 in 6.2 only;
;implemented as jump to api-ms-win-core-libraryloader-l1-2-0 in 6.3 and higher
;EnumSystemLocalesEx	implemented as jump to KERNELBASE in 6.1 and higher
;EnumTimeFormatsEx	implemented as jump to KERNELBASE in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l2-1-0 in 6.2 and higher
;FindFirstFileNameTransactedW	 
;FindFirstFileNameW	implemented as jump to api-ms-win-core-file-l1-2-2 in 10.0 and higher
;FindFirstFileTransactedA	 
;FindFirstFileTransactedW	 
;FindFirstStreamTransactedW	 
;FindNLSString	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;FindNLSStringEx	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;FindNextFileNameW	implemented as jump to api-ms-win-core-file-l1-2-2 in 10.0 and higher
;FlushProcessWriteBuffers	forwarded to NTDLL function NtFlushProcessWriteBuffers
;FreeLibraryWhenCallbackReturns	forwarded to NTDLL function TpCallbackUnloadDllOnCompletion
;GetApplicationRecoveryCallback	implemented as jump to API-MS-Win-Core-WindowsErrorReporting-L1-1-0 in 6.2 and higher
;GetApplicationRestartSettings	implemented as jump to API-MS-Win-Core-WindowsErrorReporting-L1-1-0 in 6.2 and higher
;GetCalendarDateFormat	 
;GetCalendarDateFormatEx	undocumented until 2009
;GetCalendarDaysInMonth	 
;GetCalendarDifferenceInDays	 
;GetCalendarInfoEx	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;GetCalendarMonthsInYear	 
;GetCalendarSupportedDateRange	undocumented until 2009
;GetCalendarWeekNumber	 
;GetCompressedFileSizeTransactedA	 
;GetCompressedFileSizeTransactedW	 
;GetConsoleHistoryInfo	 
;GetConsoleOriginalTitleA	 
;GetConsoleOriginalTitleW	 
;GetConsoleScreenBufferInfoEx	implemented as jump to API-MS-Win-Core-Console-L2-1-0 in 6.2 and higher
;GetCurrencyFormatEx	implemented as jump to KERNELBASE in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l2-1-0 in 6.2 and higher;
;documented as “Included in Windows 95 and later” (presumably as editing mistake)
;GetCurrentConsoleFontEx	 
;GetDateFormatEx	implemented as jump to api-ms-win-core-datetime-l1-1-1 in 6.2 and higher
;GetDurationFormat	 
;GetDurationFormatEx	implemented as jump to api-ms-win-core-datetime-l1-1-2 in 10.0 and higher
;GetDynamicTimeZoneInformation	implemented as jump to API-MS-Win-Core-SysInfo-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-timezone-l1-1-0 in 6.2 and higher
;GetFileAttributesTransactedA	 
;GetFileAttributesTransactedW	 
;GetFileBandwidthReservation	 
;GetFileInformationByHandleEx	documented as available earlier through downloadable FILEEXTD.DLL;
;implemented as jump to api-ms-win-core-file-l2-1-0 in 6.2 only;
;implemented as jump to api-ms-win-core-file-l2-1-1 in 6.3 and higher
;GetFileMUIInfo	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;GetFileMUIPath	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;GetFinalPathNameByHandleA	implemented as jump to API-MS-Win-Core-File-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-file-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-file-l1-2-1 in 6.3 and higher
;GetFinalPathNameByHandleW	implemented as jump to API-MS-Win-Core-File-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-file-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-file-l1-2-1 in 6.3 and higher
;GetFullPathNameTransactedA	 
;GetFullPathNameTransactedW	 
;GetLocaleInfoEx	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;GetLongPathNameTransactedA	 
;GetLongPathNameTransactedW	 
;GetNLSVersionEx	implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;GetNamedPipeAttribute	implemented as jump to API-MS-Win-Core-NamedPipe-L1-1-0 in 6.1 only;
;implemented as jump to KERNELBASE in 6.2 and higher
;GetNamedPipeClientComputerNameA	 
;GetNamedPipeClientComputerNameW	implemented as jump to API-MS-Win-Core-NamedPipe-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-namedpipe-l1-2-0 in 6.2 and higher
;GetNamedPipeClientProcessId	 
;GetNamedPipeClientSessionId	 
;GetNamedPipeServerProcessId	 
;GetNamedPipeServerSessionId	 
;GetNumaProximityNode	 
;GetNumberFormatEx	implemented as jump to KERNELBASE in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l2-1-0 in 6.2 and higher
;GetPhysicallyInstalledSystemMemory	begins in SP1;
;implemented as jump to api-ms-win-core-sysinfo-l1-2-1 in 6.3 and higher
;GetProductInfo	implemented as jump to api-ms-win-core-sysinfo-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-sysinfo-l1-2-1 in 6.3 and higher
@ stdcall -stub -version=0x601+ GetQueuedCompletionStatusEx(ptr ptr long ptr long long) ;	implemented as jump to API-MS-Win-Core-IO-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-io-l1-1-1 in 6.2 and higher
;GetStringScripts	implemented as jump to api-ms-win-core-normalization-l1-1-0 in 6.2 and higher
;GetSystemDefaultLocaleName	implemented as jump to KERNELBASE in 6.1 and higher
;GetSystemPreferredUILanguages	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;GetThreadPreferredUILanguages	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;GetThreadUILanguage	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;implemented as jump to api-ms-win-core-sysinfo-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-sysinfo-l1-2-1 in 6.3 only
;GetTimeFormatEx	implemented as jump to api-ms-win-core-datetime-l1-1-1 in 6.2 and higher
;GetTimeZoneInformationForYear	begins in SP1;
;implemented as jump to API-MS-Win-Core-SysInfo-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-timezone-l1-1-0 in 6.2 and higher
;GetUILanguageInfo	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;GetUserDefaultLocaleName	implemented as jump to KERNELBASE in 6.1 and higher
;GetUserPreferredUILanguages	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;GetVolumeInformationByHandleW	implemented as jump to API-MS-Win-Core-File-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-file-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-file-l1-2-1 in 6.3 and higher
;IdnToAscii	documented as available earlier through downloadable DLL;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;IdnToNameprepUnicode	documented as available earlier through downloadable DLL;
;implemented as jump to api-ms-win-core-normalization-l1-1-0 in 6.2 and higher
;IdnToUnicode	documented as available earlier through downloadable DLL;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;InitOnceComplete	forwarded to API-MS-Win-Core-Synch-L1-1-0 in 6.2 and higher
;InitOnceExecuteOnce	forwarded to API-MS-Win-Core-Synch-L1-1-0 in 6.2 and higher
;InitOnceInitialize	forwarded to NTDLL function RtlRunOnceInitialize
;InitializeConditionVariable	forwarded to NTDLL function RtlInitializeConditionVariable
;InitializeCriticalSectionEx	implemented as jump to API-MS-Win-Core-Synch-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-synch-l1-2-0 in 6.2 and higher
;InitializeProcThreadAttributeList	forwarded to API-MS-Win-Core-ProcessThreads-L1-1-0 in 6.1 and higher
;InitializeSRWLock	forwarded to NTDLL function RtlInitializeSRWLock
;InterlockedPushListSList	forwarded to NTDLL function RtlInterlockedPushListSList
;IsCalendarLeapDay	 
;IsCalendarLeapMonth	 
;IsCalendarLeapYear	undocumented until 2009
;IsNormalizedString	documented as available earlier through downloadable DLL;
;implemented as jump to api-ms-win-core-normalization-l1-1-0 in 6.2 and higher
;IsThreadAFiber	implemented as jump to api-ms-win-core-fibers-l1-1-1 in 6.2 only
;IsThreadpoolTimerSet	forwarded to NTDLL function TpIsTimerSet
;IsValidCalDateTime	 
;IsValidLocaleName	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;LCIDToLocaleName	implemented as jump to KERNELBASE in 6.1 and higher
;LCMapStringEx	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;LeaveCriticalSectionWhenCallbackReturns	forwarded to NTDLL function TpCallbackLeaveCriticalSectionOnCompletion
;LoadStringBaseExW	implemented as jump to KERNELBASE in 6.1 and higher
;LoadStringBaseW	 
;LocaleNameToLCID	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;MapViewOfFileExNuma	implemented as jump to KERNELBASE in 6.1 and higher
;MoveFileTransactedA	 
;MoveFileTransactedW	 
;NlsCheckPolicy	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-private-l1-1-0 in 6.2 and higher
;NlsEventDataDescCreate	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-private-l1-1-0 in 6.2 and higher
;NlsUpdateLocale	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-private-l1-1-0 in 6.2 and higher
;NlsUpdateSystemLocale	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-private-l1-1-0 in 6.2 and higher
;NlsWriteEtwEvent	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 and higher;
;implemented as call to api-ms-win-core-localization-private-l1-1-0 in 6.2 and higher
;NormalizeString	documented as available earlier through downloadable DLL;
;implemented as jump to api-ms-win-core-normalization-l1-1-0 in 6.2 and higher
;NotifyUILanguageChange	 
;OpenFileById	implemented as jump to api-ms-win-core-file-l2-1-1 in 6.3 and higher;
;documented as available earlier through downloadable FILEEXTD.DLL
;OpenPrivateNamespaceA	 
;OpenPrivateNamespaceW	implemented as jump to API-MS-Win-Core-Namespace-L1-1-0 in 6.2 and higher
;QueryActCtxSettingsW	implemented as jump to API-MS-Win-Core-SideBySide-L1-1-0 in 6.2 and higher
;QueryFullProcessImageNameA	implemented as jump to api-ms-win-core-psapi-ansi-l1-1-0 in 6.2 and higher
;QueryFullProcessImageNameW	implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;QueryIdleProcessorCycleTime	implemented as jump to api-ms-win-core-realtime-l1-1-0 in 6.2 and higher
;QueryProcessAffinityUpdateMode	begins in SP1;
;implemented as jump to API-MS-Win-Core-ProcessThreads-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-processthreads-l1-1-1 in 6.2 only;
;implemented as jump to api-ms-win-core-processthreads-l1-1-2 in 6.3 and higher
;QueryProcessCycleTime	implemented as jump to api-ms-win-core-realtime-l1-1-0 in 6.2 and higher
;QueryThreadCycleTime	implemented as jump to api-ms-win-core-realtime-l1-1-0 in 6.2 and higher
;RegisterApplicationRecoveryCallback	 
;RegisterApplicationRestart	 
;ReleaseMutexWhenCallbackReturns	forwarded to NTDLL function TpCallbackReleaseMutexOnCompletion
;ReleaseSRWLockExclusive	forwarded to NTDLL function RtlReleaseSRWLockExclusive
;ReleaseSRWLockShared	forwarded to NTDLL function RtlReleaseSRWLockShared
;ReleaseSemaphoreWhenCallbackReturns	forwarded to NTDLL function TpCallbackReleaseSemaphoreOnCompletion
;RemoveDirectoryTransactedA	 
;RemoveDirectoryTransactedW	 
;RemoveSecureMemoryCacheCallback	begins in SP1;
;implemented as wrapper around NTDLL function RtlDeregisterSecureMemoryCacheCallback
;ReplacePartitionUnit	begins in SP1;
;x86 only
;SetConsoleHistoryInfo	 
;SetConsoleScreenBufferInfoEx	implemented as jump to API-MS-Win-Core-Console-L2-1-0 in 6.2 and higher
;SetCurrentConsoleFontEx	 
;SetDynamicTimeZoneInformation	implemented as jump to api-ms-win-core-timezone-l1-1-0 in 6.2 and higher
;SetEventWhenCallbackReturns	forwarded to NTDLL function TpCallbackSetEventOnCompletion
;SetFileAttributesTransactedA	 
;SetFileAttributesTransactedW	 
;SetFileBandwidthReservation	 
;SetFileInformationByHandle	documented as available earlier through downloadable FILEEXTD.DLL;
;implemented as jump to API-MS-Win-Core-File-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-file-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-file-l1-2-1 in 6.3 and higher
;SetFileIoOverlappedRange	implemented as jump to api-ms-win-core-file-l1-2-1 in 6.3 and higher
;SetNamedPipeAttribute	 
;SetProcessAffinityUpdateMode	begins in SP1;
;implemented as jump to API-MS-Win-Core-ProcessThreads-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-processthreads-l1-1-1 in 6.2 only;
;implemented as jump to api-ms-win-core-processthreads-l1-1-2 in 6.3 and higher
;SetSearchPathMode	begins in SP2;
;implemented as wrapper around NTDLL function RtlSetSearchPathMode in 6.2 and higher
;SetStdHandleEx	implemented as jump to API-MS-Win-Core-ProcessEnvironment-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-processenvironment-l1-2-0 in 6.2 and higher
;SetThreadPreferredUILanguages	implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;SetThreadpoolThreadMaximum	forwarded to NTDLL function TpSetPoolMaxThreads
;SetThreadpoolThreadMinimum	implemented as wrapper around NTDLL function TpSetPoolMinThreads
;SetThreadpoolTimer	forwarded to NTDLL function TpSetTimer
;SetThreadpoolWait	forwarded to NTDLL function TpSetWait
;SleepConditionVariableCS	forwarded to API-MS-Win-Core-Synch-L1-1-0 in 6.2 and higher
;SleepConditionVariableSRW	forwarded to API-MS-Win-Core-Synch-L1-1-0 in 6.2 and higher
;StartThreadpoolIo	forwarded to NTDLL function TpStartAsyncIoOperation
;SubmitThreadpoolWork	forwarded to NTDLL function TpPostWork
;TrySubmitThreadpoolCallback	implemented as wrapper around NTDLL function TpSimpleTryPost
;UnregisterApplicationRecoveryCallback	 
;UnregisterApplicationRestart	 
;UpdateCalendarDayOfWeek	undocumented until 2009
;UpdateProcThreadAttribute	forwarded to API-MS-Win-Core-ProcessThreads-L1-1-0 in 6.1 and higher
;VerifyScripts	implemented as jump to api-ms-win-core-normalization-l1-1-0 in 6.2 and higher
;VirtualAllocExNuma	implemented as jump to KERNELBASE in 6.1 to 6.2;
;implemented as jump to api-ms-win-core-memory-l1-1-2 in 6.3 and higher
;WaitForThreadpoolIoCallbacks	forwarded to NTDLL function TpWaitForIoCompletion
;WaitForThreadpoolTimerCallbacks	forwarded to NTDLL function TpWaitForTimer
;WaitForThreadpoolWaitCallbacks	forwarded to NTDLL function TpWaitForWait
;WaitForThreadpoolWorkCallbacks	forwarded to NTDLL function TpWaitForWork
;WakeAllConditionVariable	forwarded to NTDLL function RtlWakeAllConditionVariable
;WakeConditionVariable	forwarded to NTDLL function RtlWakeConditionVariable
;WerGetFlags	 
;WerRegisterFile	implemented as jump to API-MS-Win-Core-WindowsErrorReporting-L1-1-0 in 6.2 and higher
;WerRegisterMemoryBlock	implemented as jump to API-MS-Win-Core-WindowsErrorReporting-L1-1-0 in 6.2 and higher
;WerSetFlags	 
;WerUnregisterFile	implemented as jump to API-MS-Win-Core-WindowsErrorReporting-L1-1-0 in 6.2 and higher
;WerUnregisterMemoryBlock	implemented as jump to API-MS-Win-Core-WindowsErrorReporting-L1-1-0 in 6.2 and higher
;WerpCleanupMessageMapping	 
;WerpInitiateRemoteRecovery	 
;WerpNotifyLoadStringResource	implemented as jump to API-MS-Win-Core-WindowsErrorReporting-L1-1-0 in 6.2 and higher
;WerpNotifyLoadStringResourceEx	 
;WerpNotifyUseStringResource	implemented as jump to API-MS-Win-Core-WindowsErrorReporting-L1-1-0 in 6.2 and higher
;WerpStringLookup	 
;Wow64GetThreadContext	implemented as wrapper around NTDLL function RtlWow64GetThreadContext (x64 only)
;Wow64SetThreadContext	implemented as wrapper around NTDLL function RtlWow64SetThreadContext (x64 only)
;Wow64SuspendThread	implemented as wrapper around NTDLL function RtlWow64SuspendThread (x64 only)
;NT6.1
;@ stdcall -version=0x601+ AddIntegrityLabelToBoundaryDescriptor() ntdll.RtlAddIntegrityLabelToBoundaryDescriptor
@ stub -version=0x601+ BaseCheckAppcompatCacheEx
@ stub -version=0x601+ BaseDllReadWriteIniFile	 
@ stdcall -version=0x601+ BaseFormatObjectAttributes(ptr ptr ptr)
@ stdcall -version=0x601+ BaseFormatTimeOut(ptr long)
@ stdcall -version=0x601+ BaseSetLastNTError(long) 
@ stub -version=0x601+ BaseVerifyUnicodeString	 
@ stdcall -version=0x601+ Basep8BitStringToDynamicUnicodeString(ptr str)
@ stdcall -version=0x601+ BasepAllocateActivationContextActivationBlock(long ptr ptr ptr)	 
@ stub -version=0x601+ BasepAnsiStringToDynamicUnicodeString	 
@ stub -version=0x601+ BasepCheckAppCompat
;@ stub -version=0x601+ BasepFreeActivationContextActivationBlock	 
;@ stub -version=0x601+ BasepMapModuleHandle
@ stub -version=0x601+ CopyContext
;CopyExtendedContext	discontinued in Windows 7 SP1
;@ stdcall -version=0x601+ CreateProcessAsUserW() ADVAPI32.CreateProcessAsUserW
@ stdcall -stub -version=0x601+ CreateRemoteThreadEx(ptr ptr long ptr ptr long ptr ptr)
@ stub -version=0x601+ -arch=x86_64 CreateUmsCompletionList
@ stub -version=0x601+ -arch=x86_64 CreateUmsThreadContext
@ stub -version=0x601+ -arch=x86_64 DeleteUmsCompletionList
@ stub -version=0x601+ -arch=x86_64 DeleteUmsThreadContext
@ stdcall -stub -version=0x601+ -arch=x86_64 DequeueUmsCompletionListItems(ptr long ptr)
@ stub -version=0x601+ DisableThreadProfiling
@ stub -version=0x601+ EnableThreadProfiling
@ stub -version=0x601+ -arch=x86_64 EnterUmsSchedulingMode ;RtlEnterUmsSchedulingMode
@ stub -version=0x601+ -arch=x86_64 ExecuteUmsThread
@ stdcall -stub -version=0x601+ FindStringOrdinal(long ptr long ptr long long)
@ stdcall -version=0x601+ GetActiveProcessorCount(long)
@ stdcall -version=0x601+ GetActiveProcessorGroupCount()
@ stdcall -version=0x601+ GetCurrentProcessorNumberEx() ntdll.RtlGetCurrentProcessorNumberEx
;GetCurrentUmsThread	x64 only;
@ stdcall -stub -version=0x601+ GetEnabledXStateFeatures()
@ stub -version=0x601+ GetEraNameCountedString	;implemented as jump to KERNELBASE
@ stdcall -version=0x601+ GetLogicalProcessorInformationEx(ptr ptr ptr)
@ stdcall -version=0x601+ GetMaximumProcessorCount(long)
@ stdcall -version=0x601+ GetMaximumProcessorGroupCount()
;GetNextUmsListItem	x64 only; 
@ stdcall -version=0x601+ GetProcessGroupAffinity(ptr ptr ptr)	;implemented as jump to API-MS-Win-Core-ProcessTopology-L1-1-0 in 6.2 only;
@ stdcall -version=0x601 GetProcessPreferredUILanguages(long ptr ptr ptr) ;	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only; implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only; implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
@ stub -version=0x601 GetProcessUserModeExceptionPolicy
;GetProcessorSystemCycleTime	forwarded to api-ms-win-core-sysinfo-l1-2-2
;GetThreadErrorMode	implemented as jump to api-ms-win-core-errorhandling-l1-1-3 in 10.0 and higher
@ stdcall -stub -version=0x601+ GetThreadGroupAffinity(ptr ptr)	;implemented as jump to API-MS-Win-Core-ProcessTopology-L1-1-0 in 6.2 only;
;implemented as jump to api-ms-win-core-processtopology-l1-2-0 in 6.3 and higher
;GetThreadIdealProcessorEx	implemented as jump to api-ms-win-core-processthreads-l1-1-1 in 6.2 only;
;implemented as jump to api-ms-win-core-processthreads-l1-1-2 in 6.3 and higher
;GetUmsCompletionListEvent	x64 only;
;implemented as wrapper around NTDLL function RtlGetUmsCompletionListEvent
;GetUmsSystemThreadInformation	x64 only;
;begins in Windows 7 SP1
;GetXStateFeaturesMask	begins in Windows 7 SP1;
;implemented as jump to api-ms-win-core-xstate-l2-1-0 in 6.3 and higher
;InitializeContext	begins in Windows 7 SP1;
;implemented as jump to api-ms-win-core-xstate-l2-1-0 in 6.3 and higher
;InitializeExtendedContext	discontinued in Windows 7 SP1
;K32EmptyWorkingSet	whole implementation of EmptyWorkingSet for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;K32EnumDeviceDrivers	whole implementation of EnumDeviceDrivers for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;K32EnumPageFilesA	whole implementation of EnumPageFilesA for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-ansi-l1-1-0 in 6.2 and higher
;K32EnumPageFilesW	whole implementation of EnumPageFilesW for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;K32EnumProcessModules	whole implementation of EnumProcessModules for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-obsolete-l1-1-0 in 6.2 to 6.3;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 10.0 and higher
;K32EnumProcessModulesEx	whole implementation of EnumProcessModulesEx for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-obsolete-l1-1-0 in 6.2 to 6.3;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 10.0 and higher
;K32EnumProcesses	whole implementation of EnumProcesses for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;K32GetDeviceDriverBaseNameA	whole implementation of GetDeviceDriverBaseNameA for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-ansi-l1-1-0 in 6.2 and higher
;K32GetDeviceDriverBaseNameW	whole implementation of GetDeviceDriverBaseNameW for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;K32GetDeviceDriverFileNameA	whole implementation of GetDeviceDriverFileNameA for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-ansi-l1-1-0 in 6.2 and higher
;K32GetDeviceDriverFileNameW	whole implementation of GetDeviceDriverFileNameW for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;K32GetMappedFileNameA	whole implementation of GeMappedFileNameW for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-ansi-l1-1-0 in 6.2 and higher
;K32GetMappedFileNameW	whole implementation of GetMappedFileNameW for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;K32GetModuleBaseNameA	whole implementation of GetModuleBaseNameA for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-obsolete-l1-1-0 in 6.2 to 6.3;
;implemented as jump to api-ms-win-core-psapi-ansi-l1-1-0 in 10.0 and higher
;K32GetModuleBaseNameW	whole implementation of GetModuleBaseNameW for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-obsolete-l1-1-0 in 6.2 to 6.3;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 10.0 and higher
;K32GetModuleFileNameExA	whole implementation of GetModuleFileNameExA for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-obsolete-l1-1-0 in 6.2 to 6.3;
;implemented as jump to api-ms-win-core-psapi-ansi-l1-1-0 in 10.0 and higher
;K32GetModuleFileNameExW	whole implementation of GetModuleFileNameExW for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-obsolete-l1-1-0 in 6.2 to 6.3;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 10.0 and higher
;K32GetModuleInformation	whole implementation of GetModuleInformation for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-obsolete-l1-1-0 in 6.2 to 6.3;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 10.0 and higher
;K32GetPerformanceInfo	whole implementation of GetPerformanceInfo for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;K32GetProcessImageFileNameA	whole implementation of GetProcessImageFileNameA for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-ansi-l1-1-0 in 6.2 and higher
;K32GetProcessImageFileNameW	whole implementation of GetProcessImageFileNameW for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;K32GetProcessMemoryInfo	whole implementation of GetProcessMemoryInfo for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;K32GetWsChanges	whole implementation of GetWsChanges for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;K32GetWsChangesEx	whole implementation of GetWsChangesEx for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;K32InitializeProcessForWsWatch	whole implementation of InitializeProcessForWsWatch for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;K32QueryWorkingSet	whole implementation of QueryWorkingSet for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;K32QueryWorkingSetEx	whole implementation of QueryWorkingSetEx for PSAPI 6.1 and higher;
;implemented as jump to api-ms-win-core-psapi-l1-1-0 in 6.2 and higher
;LoadAppInitDlls	 
;LocateExtendedFeature	forwarded to API-MS-Win-Core-XState-L1-1-0 function RtlLocateExtendedFeature;
;discontinued in Windows 7 SP1
;LocateLegacyContext	forwarded to API-MS-Win-Core-XState-L1-1-0 function RtlLocateLegacyContext;
;discontinued in Windows 7 SP1
;LocateXStateFeature	begins in Windows 7 SP1;
;implemented as jump to api-ms-win-core-xstate-l2-1-0 in 6.3 and higher
;NotifyMountMgr	 
;OpenProcessToken	forwarded to API-MS-Win-Core-ProcessThreads-L1-1-0;
;also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher
;OpenThreadToken	forwarded to API-MS-Win-Core-ProcessThreads-L1-1-0;
;also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher
;PowerClearRequest	 
;PowerCreateRequest	 
;PowerSetRequest	 
;QueryIdleProcessorCycleTimeEx	implemented as jump to api-ms-win-core-realtime-l1-1-0 in 6.2 and higher
;QueryThreadProfiling	implemented as wrapper around NTDLL function RtlQueryThreadProfiling
;QueryThreadpoolStackInformation	implemented as wrapper around NTDLL function TpQueryPoolStackInformation
;QueryUmsThreadInformation	x64 only;
;implemented as wrapper around NTDLL function RtlQueryUmsThreadInformation
@ stub -version=0x601+ QueryUnbiasedInterruptTime
;RaiseFailFastException	forwarded to KERNELBASE in 10.0 and higher
;ReadThreadProfilingData	implemented as wrapper around NTDLL function RtlReadThreadProfilingData
;RegCloseKey	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegCreateKeyExA	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegCreateKeyExW	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegDeleteKeyExA	also named export from ADVAPI32 5.2 from Windows Server 2003 SP1, and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegDeleteKeyExW	also named export from ADVAPI32 5.2 from Windows Server 2003 SP1, and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegDeleteTreeA	also named export from ADVAPI32 6.0 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegDeleteTreeW	also named export from ADVAPI32 6.0 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegDeleteValueA	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegDeleteValueW	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegDisablePredefinedCacheEx	also named export from ADVAPI32 5.1 from Windows XP SP3, and 6.0 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegEnumKeyExA	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegEnumKeyExW	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegEnumValueA	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegEnumValueW	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegFlushKey	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegGetKeySecurity	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegGetValueA	also named export from ADVAPI32 5.2 from Windows Server 2003 SP1, and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegGetValueW	also named export from ADVAPI32 5.2 from Windows Server 2003 SP1, and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegKrnGetGlobalState	discontinued in 6.2
;RegKrnInitialize	discontinued in 6.2
;RegLoadKeyA	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegLoadKeyW	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegLoadMUIStringA	also named export from ADVAPI32 6.0 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegLoadMUIStringW	also named export from ADVAPI32 6.0 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegNotifyChangeKeyValue	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegOpenCurrentUser	also named export from ADVAPI32 5.0 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegOpenKeyExA	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegOpenKeyExW	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegOpenUserClassesRoot	also named export from ADVAPI32 5.0 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegQueryInfoKeyA	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegQueryInfoKeyW	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegQueryValueExA	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegQueryValueExW	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegRestoreKeyA	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegRestoreKeyW	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegSaveKeyExA	also named export from ADVAPI32 5.1 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegSaveKeyExW	also named export from ADVAPI32 5.1 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegSetKeySecurity	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegSetValueExA	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegSetValueExW	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegUnLoadKeyA	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;RegUnLoadKeyW	also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0 in 6.2 and higher
;ResolveLocaleName	implemented as jump to API-MS-Win-Core-Localization-L1-1-0 in 6.1 only;
;implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;SetExtendedFeaturesMask	forwarded to API-MS-Win-Core-XState-L1-1-0 function RtlSetExtendedFeaturesMask;
;discontinued in Windows 7 SP1
;SetProcessPreferredUILanguages	implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;SetProcessUserModeExceptionPolicy	begins in Windows 7 SP1;
;discontinued in 6.2
;SetThreadErrorMode	implemented as jump to api-ms-win-core-errorhandling-l1-1-3 in 10.0 and higher
;SetThreadGroupAffinity	implemented as jump to API-MS-Win-Core-ProcessTopology-L1-1-0 in 6.2 only;
;implemented as jump to api-ms-win-core-processtopology-l1-2-0 in 6.3 and higher
;SetThreadIdealProcessorEx	implemented as jump to api-ms-win-core-processthreads-l1-1-1 in 6.2 only;
;implemented as jump to api-ms-win-core-processthreads-l1-1-2 in 6.3 and higher
;SetThreadToken	forwarded to API-MS-Win-Core-ProcessThreads-L1-1-0;
;also named export from ADVAPI32 3.51 and higher;
;whole implementation for ADVAPI32 6.1 and higher
;SetThreadpoolStackInformation	implemented as wrapper around NTDLL function TpSetPoolStackInformation
;SetUmsThreadInformation	x64 only;
;implemented as wrapper around NTDLL function RtlSetUmsThreadInformation
;SetWaitableTimerEx	forwarded to API-MS-Win-Core-ThreadPool-L1-1-0 in 6.1;
;forwarded to API-MS-Win-Core-Synch-L1-1-0 in 6.2 and higher
;SetXStateFeaturesMask	begins in Windows 7 SP1;
;implemented as jump to api-ms-win-core-xstate-l2-1-0 in 6.3 and higher
;SortCloseHandle	 
;SortGetHandle	 
;TryAcquireSRWLockExclusive	forwarded to NTDLL function RtlTryAcquireSRWLockExclusive
;TryAcquireSRWLockShared	forwarded to NTDLL function RtlTryAcquireSRWLockShared
;UmsThreadYield	x64 only;
;implemented as wrapper around NTDLL function RtlUmsThreadYield
;WerRegisterRuntimeExceptionModule	implemented as jump to API-MS-Win-Core-WindowsErrorReporting-L1-1-0 in 6.2 and higher
;WerUnregisterRuntimeExceptionModule	implemented as jump to API-MS-Win-Core-WindowsErrorReporting-L1-1-0 in 6.2 and higher
;Wow64GetThreadSelectorEntry

; NT6.2
@ stub -version=0x602 AcquireStateLock
@ stub -version=0x602+ ActivateActCtxWorker	 
@ stdcall -version=0x602+ AddDllDirectory() ntdll.LdrAddDllDirectory
@ stdcall -version=0x602+ AddRefActCtxWorker() ntdll.RtlAddRefActivationContext
;AddResourceAttributeAce	implemented as jump to api-ms-win-security-base-l1-2-0
;AddScopedPolicyIDAce	implemented as jump to api-ms-win-security-base-l1-2-0
@ stub -version=0x602 AppContainerDeriveSidFromMoniker
@ stub -version=0x602 AppContainerFreeMemory
@ stub -version=0x602 AppContainerLookupDisplayNameMrtReference
@ stub -version=0x602 AppContainerLookupMoniker
@ stub -version=0x602 AppContainerRegisterSid
@ stub -version=0x602 AppContainerUnregisterSid
@ stub -version=0x602 AppXFreeMemory
@ stub -version=0x602 AppXGetApplicationData
@ stub -version=0x602 AppXGetDevelopmentMode
;@ stub -version=0x602 AppXGetOSMaxVersionTested	forwarded to KERNELBASE in 6.3 and higher
@ stub -version=0x602 AppXGetOSMinVersion
@ stub -version=0x602 AppXGetPackageCapabilities
@ stub -version=0x602 AppXGetPackageSid
@ stub -version=0x602 AppXGetPackageState
@ stub -version=0x602 AppXLookupDisplayName
@ stub -version=0x602 AppXLookupMoniker
@ stub -version=0x602 AppXSetPackageState
@ stub -version=0x602+ BaseCheckAppcompatCacheExWorker	 
@ stub -version=0x602+ BaseCheckAppcompatCacheWorker	 
@ stub -version=0x602+ BaseCheckElevation	 
@ stub -version=0x602+ BaseCleanupAppcompatCacheSupportWorker	 
@ stdcall -version=0x602+ BaseDestroyVDMEnvironment(ptr ptr)	 
@ stub -version=0x602+ BaseDumpAppcompatCacheWorker	 
@ stub -version=0x602+ BaseElevationPostProcessing	 
@ stub -version=0x602+ BaseFlushAppcompatCacheWorker	 
@ stub -version=0x602+ BaseInitAppcompatCacheSupportWorker	 
@ stub -version=0x602+ BaseIsAppcompatInfrastructureDisabledWorker	 
@ stdcall -version=0x602+ BaseIsDosApplication(ptr long)	 
@ stub -version=0x602+ BaseUpdateAppcompatCacheWorker	 
@ stdcall -version=0x602+ BaseUpdateVDMEntry(long ptr long long)	 
@ stub -version=0x602+ BaseWriteErrorElevationRequiredEvent	 
@ stub -version=0x602 BasepAppCompatHookDLL
;@ stub -version=0x602+ BasepAppContainerEnvironmentExtension	 
;@ stub -version=0x602+ BasepAppXExtension	 
;@ stub -version=0x602+ BasepCheckWebBladeHashes	 
;@ stub -version=0x602+ BasepConstructSxsCreateProcessMessage	 
;@ stub -version=0x602+ BasepCopyEncryption	 
;@ stub -version=0x602+ BasepGetAppCompatData	 
;@ stub -version=0x602+ BasepGetComputerNameFromNtPath	 
;@ stub -version=0x602+ BasepGetExeArchType	 
;@ stub -version=0x602+ BasepIsProcessAllowed	 
;@ stub -version=0x602+ BasepNotifyLoadStringResource	 
;BasepPostSuccessAppXExtension	implemented as jump to KERNELBASE function AppXPostSuccessExtension in 6.3 and higher
@ stub -version=0x602+ BasepProcessInvalidImage	 
@ stub -version=0x602+ BasepQueryAppCompat	 
;BasepReleaseAppXContext	implemented as jump to KERNELBASE function AppXReleaseAppXContext in 6.3 and higher
@ stub -version=0x602+ BasepReleaseSxsCreateProcessUtilityStruct	 
@ stub -version=0x602+ BasepReportFault	 
@ stub -version=0x602+ BasepSetFileEncryptionCompression	 
;CeipIsOptedIn	forwarded to KERNELBASE in 10.0 and higher;
@ stub -version=0x602+ CheckAllowDecryptedRemoteDestinationPolicy	 
@ stub -version=0x602+ CheckForReadOnlyResourceFilter	 
;CheckTokenCapability	implemented as jump to api-ms-win-security-base-l1-2-0
;CheckTokenMembershipEx	implemented as jump to api-ms-win-security-base-l1-2-0
;ClosePackageInfo	forwarded to KERNELBASE in 6.3 and higher
;CloseState	forwarded to KERNELBASE in 6.3 and higher
@ stub -version=0x602 CloseStateAtom
@ stub -version=0x602 CloseStateChangeNotification
@ stub -version=0x602 CloseStateContainer
@ stub -version=0x602 CloseStateLock
@ stub -version=0x602 CommitStateAtom
;CopyFile2	implemented as jump to api-ms-win-core-file-l2-1-0 in 6.2 only;
@ stub -version=0x602+ CreateActCtxWWorker	 
;CreateFile2	implemented as jump to api-ms-win-core-file-l1-2-0 in 6.2 only;
;CreateFileMappingFromApp	forwarded to api-ms-win-core-memory-l1-1-1
@ stub -version=0x602 CreateStateAtom
@ stub -version=0x602 CreateStateChangeNotification
@ stub -version=0x602 CreateStateContainer
@ stub -version=0x602 CreateStateLock
@ stub -version=0x602 CreateStateSubcontainer
@ stub -version=0x602 DeactivateActCtxWorker	 
@ stub -version=0x602 DeleteStateAtomValue
@ stub -version=0x602 DeleteStateContainer
@ stub -version=0x602 DeleteStateContainerValue
@ stub -version=0x602+ DuplicateEncryptionInfoFileExt	 
@ stub -version=0x602 DuplicateStateContainerHandle
@ stub -version=0x602 EnumerateStateAtomValues
@ stub -version=0x602 EnumerateStateContainerItems
;FindActCtxSectionGuidWorker	 
;FindActCtxSectionStringWWorker	 
;GetAppContainerAce	implemented as jump to api-ms-win-security-base-l1-2-0
;GetAppContainerNamedObjectPath	implemented as jump to api-ms-win-security-appcontainer-l1-1-0
;GetApplicationRecoveryCallbackWorker	 
;GetApplicationRestartSettingsWorker	 
;GetApplicationUserModelId	forwarded to KERNELBASE in 6.3 and higher;
;documented without version information
;GetCachedSigningLevel	implemented as jump to api-ms-win-security-base-l1-2-0
;GetCurrentActCtxWorker	 
;GetCurrentApplicationUserModelId	forwarded to KERNELBASE in 6.3 and higher;
;documented without version information
;GetCurrentPackageFamilyName	forwarded to KERNELBASE in 6.3 and higher
;GetCurrentPackageFullName	forwarded to KERNELBASE in 6.3 and higher
;GetCurrentPackageId	forwarded to KERNELBASE in 6.3 and higher
;GetCurrentPackageInfo	forwarded to KERNELBASE in 6.3 and higher
;GetCurrentPackagePath	forwarded to KERNELBASE in 6.3 and higher
;GetCurrentThreadStackLimits	forwarded to API-MS-Win-Core-ProcessThreads-L1-1-0
;GetDateFormatAWorker	 
;GetDateFormatWWorker	 
;GetFirmwareEnvironmentVariableExA	 
;GetFirmwareEnvironmentVariableExW	 
;GetFirmwareType	 
;GetHivePath	discontinued in 6.3
;GetMemoryErrorHandlingCapabilities	implemented as jump to api-ms-win-core-memory-l1-1-2 in 6.3 and higher
;GetOverlappedResultEx	forwarded to api-ms-win-core-io-l1-1-1
;GetPackageFamilyName	forwarded to KERNELBASE in 6.3 and higher
;GetPackageFullName	forwarded to KERNELBASE in 6.3 and higher
;GetPackageId	forwarded to KERNELBASE in 6.3 and higher
;GetPackageInfo	forwarded to KERNELBASE in 6.3 and higher
;GetPackagePath	forwarded to KERNELBASE in 6.3 and higher
;GetPackagesByPackageFamily	forwarded to KERNELBASE in 6.3 and higher
;GetProcessInformation	implemented as jump to api-ms-win-core-processthreads-l1-1-3 in 10.0 and higher
;GetProcessMitigationPolicy	forwarded to api-ms-win-core-processthreads-l1-1-1
;GetRoamingLastObservedChangeTime	discontinued in 6.3
;GetSerializedAtomBytes	discontinued in 6.3
;GetStateContainerDepth	discontinued in 6.3
;GetStateFolder	forwarded to KERNELBASE in 6.3 and higher
;GetStateRootFolder	discontinued in 6.3
;GetStateSettingsFolder	discontinued in 6.3
;GetStateVersion	discontinued in 6.3
;GetSystemAppDataFolder	discontinued in 6.3
;GetSystemAppDataKey	forwarded to KERNELBASE in 6.3 and higher
;GetSystemTimePreciseAsFileTime	implemented as jump to api-ms-win-core-sysinfo-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-sysinfo-l1-2-1 in 6.3 and higher
;GetThreadInformation	implemented as jump to api-ms-win-core-processthreads-l1-1-2 in 6.3 and higher
;GetTimeFormatAWorker	 
;GetTimeFormatWWorker	 
;GlobalAddAtomExA	documented without version information
;GlobalAddAtomExW	documented without version information
;InterlockedPushListSListEx	forwarded to NTDLL function RtlInterlockedPushListSListEx
;IsNativeVhdBoot	 
;IsValidNLSVersion	implemented as jump to api-ms-win-core-localization-l1-2-0 in 6.2 only;
;implemented as jump to api-ms-win-core-localization-l1-2-1 in 6.3 and higher
;LoadPackagedLibrary	implemented as jump to api-ms-win-core-libraryloader-l2-1-0 in 10.0 and higher
;MapViewOfFileFromApp	forwarded to api-ms-win-core-memory-l1-1-1
;NtVdm64CreateProcessInternalW	 
;OpenConsoleWStub	 
;OpenPackageInfoByFullName	forwarded to KERNELBASE in 6.3 and higher
;OpenState	forwarded to KERNELBASE in 6.3 and higher
;OpenStateAtom	discontinued in 6.3
;OpenStateExplicit	forwarded to KERNELBASE in 6.3 and higher
;OverrideRoamingDataModificationTimesInRange	discontinued in 6.3
;PackageFamilyNameFromFullName	forwarded to KERNELBASE in 6.3 and higher
;PackageFamilyNameFromId	forwarded to KERNELBASE in 6.3 and higher
;PackageFullNameFromId	forwarded to KERNELBASE in 6.3 and higher
;PackageIdFromFullName	forwarded to KERNELBASE in 6.3 and higher
;PackageNameAndPublisherIdFromFamliyName	forwarded to KERNELBASE in 6.3 and higher
;PrefetchVirtualMemory	forwarded to api-ms-win-core-memory-l1-1-1
;PublishStateChangeNotification	discontinued in 6.3
;QueryActCtxSettingsWWorker	 
;QueryActCtxWWorker	 
;QueryStateAtomValueInfo	discontinued in 6.3
;QueryStateContainerItemInfo	discontinued in 6.3
;RaiseInvalid16BitExeError	 
;ReadStateAtomValue	discontinued in 6.3
;ReadStateContainerValue	discontinued in 6.3
;RegCopyTreeW	also named export from ADVAPI32 6.0 and higher;
;implemented as jump to api-ms-win-core-registry-l1-1-0
;RegisterBadMemoryNotification	implemented as jump to api-ms-win-core-memory-l1-1-2 in 6.3 and higher
;RegisterStateChangeNotification	discontinued in 6.3
;RegisterStateLock	discontinued in 6.3
;ReleaseActCtxWorker	implemented as jump to NTDLL function RtlReleaseActivationContext
;ReleaseStateLock	discontinued in 6.3
;RemoveDllDirectory	forwarded to API-MS-Win-Core-LibraryLoader-L1-1-0;
;documented for earlier versions (Windows Vista and higher) if KB2533623 installed
;ResetState	discontinued in 6.3
;ResolveDelayLoadedAPI	forwarded to NTDLL function LdrResolveDelayLoadedAPI;
;documented without version information
;ResolveDelayLoadsFromDll	forwarded to NTDLL function LdrResolveDelayLoadsFromDll;
;documented without version information
;SetCachedSigningLevel	implemented as jump to api-ms-win-security-base-l1-2-0
;SetDefaultDllDirectories	forwarded to API-MS-Win-Core-LibraryLoader-L1-1-0;
;documented for earlier versions (Windows Vista and higher) if KB2533623 installed
;SetFirmwareEnvironmentVariableExA	 
;SetFirmwareEnvironmentVariableExW	 
;SetProcessInformation	implemented as jump to api-ms-win-core-processthreads-l1-1-3 in 10.0 and higher
;SetProcessMitigationPolicy	forwarded to api-ms-win-core-processthreads-l1-1-1
;SetRoamingLastObservedChangeTime	discontinued in 6.3
;SetStateVersion	discontinued in 6.3
;SetThreadInformation	implemented as jump to api-ms-win-core-processthreads-l1-1-2 in 6.3 and higher
;SetThreadpoolTimerEx	forwarded to NTDLL function TpSetTimerEx;
;documented for Windows Vista and higher
;SetThreadpoolWaitEx	forwarded to NTDLL function TpSetWaitEx;
;documented as requiring Windows 10 and higher
;SetVolumeMountPointWStub	 
;SubscribeStateChangeNotification	discontinued in 6.3
;SystemTimeToTzSpecificLocalTimeEx	forwarded to api-ms-win-core-timezone-l1-1-0;
;documented for Windows 7 and higher
;TermsrvConvertSysRootToUserDir	 
;TermsrvCreateRegEntry	 
;TermsrvDeleteKey	 
;TermsrvDeleteValue	 
;TermsrvGetPreSetValue	 
;TermsrvGetWindowsDirectoryA	 
;TermsrvGetWindowsDirectoryW	 
;TermsrvOpenRegEntry	 
;TermsrvOpenUserClasses	 
;TermsrvRestoreKey	 
;TermsrvSetKeySecurity	 
;TermsrvSetValueKey	 
;TermsrvSyncUserIniFileExt	 
;TzSpecificLocalTimeToSystemTimeEx	forwarded to api-ms-win-core-timezone-l1-1-0;
;documented for Windows 7 and higher
;UnmapViewOfFileEx	forwarded to api-ms-win-core-memory-l1-1-1
;UnregisterBadMemoryNotification	implemented as wrapper around NTDLL function RtlUnsubscribeWnfStateChangeNotification in 6.2 only;
;implemented as jump to api-ms-win-core-memory-l1-1-2 in 6.3 and higher
;UnregisterStateChangeNotification	discontinued in 6.3
;UnregisterStateLock	discontinued in 6.3
;UnsubscribeStateChangeNotification	discontinued in 6.3
;WerRegisterFileWorker	 
;WerRegisterMemoryBlockWorker	 
;WerRegisterRuntimeExceptionModuleWorker	 
;WerUnregisterFileWorker	 
;WerUnregisterMemoryBlockWorker	 
;WerUnregisterRuntimeExceptionModuleWorker	 
;WerpGetDebugger	 
;WerpLaunchAeDebug	 
;WerpNotifyLoadStringResourceWorker	 
;WerpNotifyUseStringResourceWorker	 
;WriteStateAtomValue	discontinued in 6.3
;WriteStateContainerValue	discontinued in 6.3
;ZombifyActCtxWorker	implemented as wrapper around NTDLL function RtlZombifyActivationContext
@ stub -version=0x602+ timeBeginPeriod	 
@ stub -version=0x602+ timeEndPeriod	 
@ stub -version=0x602+ timeGetDevCaps	 
@ stub -version=0x602+ timeGetSystemTime	 
@ stub -version=0x602+ timeGetTime	 