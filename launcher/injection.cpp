#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <Psapi.h>
#include <iostream>
#include "injection.h"

int injectDll(DWORD processId, TCHAR* dllPath) {
	HANDLE processHandle;
	HANDLE remoteThread;
	LPVOID baseAddress;
	int len = wcslen(dllPath);
	char* mbPath = new char[len];
	size_t mbPathLen = 0;
	wcstombs_s(&mbPathLen, mbPath, len + 1, dllPath, len);
	HMODULE hKernel32 = GetModuleHandle(L"Kernel32");
	if (!hKernel32) {
		return 0;
	}
	VOID* library = GetProcAddress(hKernel32, "LoadLibraryA");

	processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

	baseAddress = VirtualAllocEx(processHandle, NULL, mbPathLen, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
	if (!baseAddress) {
		return 0;
	}
	WriteProcessMemory(processHandle, baseAddress, mbPath, mbPathLen, NULL);

	remoteThread = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)library, baseAddress, 0, NULL);
	CloseHandle(processHandle);
	return 1;
}