
typedef struct _THREAD_TLS_INFORMATION
{
	ULONG      Flags;

	union
	{
		PVOID* TlsVector;
		PVOID  TlsModulePointer;
	};

	HANDLE     ThreadId;
} THREAD_TLS_INFORMATION, *PTHREAD_TLS_INFORMATION;

typedef enum _PROCESS_TLS_INFORMATION_TYPE
{
	ProcessTlsReplaceIndex,
	ProcessTlsReplaceVector,
	MaxProcessTlsOperation
} PROCESS_TLS_INFORMATION_TYPE, *PPROCESS_TLS_INFORMATION_TYPE;

typedef struct _PROCESS_TLS_INFORMATION
{
	ULONG                  Reserved;
	ULONG                  OperationType;
	ULONG                  ThreadDataCount;

	union
	{
		ULONG              TlsIndex;
		ULONG              TlsVectorLength;
	};

	THREAD_TLS_INFORMATION ThreadData[ANYSIZE_ARRAY];
} PROCESS_TLS_INFORMATION, *PPROCESS_TLS_INFORMATION;
