// dllmain.cpp : Define o ponto de entrada para o aplicativo DLL.
#include "pch.h"
#include <iostream>
#include <vector>
#include "hook.h"


typedef BOOL(__stdcall* tDeviceIoControlType)(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuffer,
	DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped);

tDeviceIoControlType deviceIoControl;

void randomizeString(char* string) {
	srand(time(0));
	int lowerBound = 97, upperBound = 122;
	for (int i = 0; i < strlen(string); i++) {
		string[i] = char(rand() % (upperBound - lowerBound + 1) + lowerBound);
	}
}

BOOL __stdcall hDeviceIoControl(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped) {
	BOOL value = deviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped);
	if (dwIoControlCode == 0x2D1400) {
		std::cout << "DeviceIoControl with control code 0x2D1400 called \n"; //todo: randomize for every control code in case it becomes a problem
		char* productId = (char*)((DWORD)lpOutBuffer + *((BYTE*)lpOutBuffer + 0x10));
		char* productRevision = (char*)((DWORD)lpOutBuffer + *((BYTE*)lpOutBuffer + 0x14));
		char* serialNumber = (char*)((DWORD)lpOutBuffer + *((BYTE*)lpOutBuffer + 0x18));
		randomizeString(productId);
		randomizeString(productRevision);
		randomizeString(serialNumber);
		return value;
	}
	return value;
}

DWORD WINAPI HackThread(HMODULE hModule)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	HMODULE hKernel32 = GetModuleHandleW(L"Kernel32");
	VOID* aDeviceIoControl = GetProcAddress(hKernel32, "DeviceIoControl");
	deviceIoControl = (tDeviceIoControlType)aDeviceIoControl;
	deviceIoControl = (tDeviceIoControlType)TrampHook32((char*)deviceIoControl, (char*)hDeviceIoControl, 5);

	while (true) {
		if (GetAsyncKeyState(VK_END) & 1) {
			fclose(f);
			FreeConsole();
			FreeLibraryAndExitThread(hModule, 0);
			return 0;
		}
	}
}
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

