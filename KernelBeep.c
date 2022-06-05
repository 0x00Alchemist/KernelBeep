#include <ntddk.h>
#include <wdf.h>

// Fuck HalMakeBeep
typedef BOOLEAN(__stdcall *HMB)(ULONG);

NTSTATUS DriverUnload(PDRIVER_OBJECT DriverObject);
NTSTATUS KeSleep(PDRIVER_OBJECT DriverObject, KPROCESSOR_MODE WaitMode, BOOLEAN Alertable, PLARGE_INTEGER Interval);

// Driver entry point
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(RegistryPath);

	// Resolve halMakeShit
	UNICODE_STRING uFuncName;
	RtlInitUnicodeString(&uFuncName, L"HalMakeBeep");
	HMB pHalMakeBeep = (HMB)MmGetSystemRoutineAddress(&uFuncName);

	// Beep
	pHalMakeBeep(0x2000);
	// Wait some time
	KeSleep(DriverObject, KernelMode, FALSE, 100);
	// Stop beep
	pHalMakeBeep(0);

	// Unload kernel module
	DriverObject->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}

// Unload driver routine
NTSTATUS DriverUnload(PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, L"Called DriverUnload routine\n"));

	return STATUS_SUCCESS;
}

// wtf Sleep in kernel
NTSTATUS KeSleep(PDRIVER_OBJECT DriverObject, KPROCESSOR_MODE WaitMode, BOOLEAN Alertable, PLARGE_INTEGER Interval) {
	UNREFERENCED_PARAMETER(DriverObject);
	KeDelayExecutionThread(WaitMode, Alertable, Interval);

	return STATUS_SUCCESS;
}
