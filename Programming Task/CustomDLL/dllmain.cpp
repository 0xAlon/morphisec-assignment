#include <Windows.h>
#include "detours.h"

#pragma comment(lib, "detours.lib")

typedef BOOL(WINAPI* mBox)(HWND, LPCSTR, LPCSTR, UINT);
mBox cMessageBoxA = NULL;

LONG error;

/*
*	Create a custom MessageBoxA to replace the string inside the pop up message.
*/
BOOL WINAPI customMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	return cMessageBoxA(hWnd, "HOOKED", lpCaption, uType);
}

/*
*	Load the custom MessageBoxA using detour library.
*/
BOOL processAttach()
{
	HMODULE user32 = GetModuleHandle(TEXT("user32.dll")); // Retrieves user32.dll module.
	if (user32 != NULL)
	{
		cMessageBoxA = (mBox)GetProcAddress(user32, "MessageBoxA"); // Retrieves the address of an MessageBoxA function and cast to mBox.
		if (cMessageBoxA != NULL)
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach((PVOID*)& cMessageBoxA, customMessageBoxA); // Attach a detour to a target function.
			error = DetourTransactionCommit(); // Commit the current transaction for attaching detours.
			if (error != NO_ERROR) {
				return 1;
			}
		}
	}
}

/*
*	Unloaded from the virtual address space and detach a detour from a target function.
*/
BOOL processDetach()
{
	if (cMessageBoxA != NULL)
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach((PVOID*)& cMessageBoxA, customMessageBoxA); // Detach a detour from a target function.
		error = DetourTransactionCommit(); // Commit the current transaction for detaching detours.
		if (error != NO_ERROR) {
			return 1;
		}

	}
}

/*
* DLL Main Entry Point.
*/
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL); // disable the DLL_THREAD_ATTACH and DLL_THREAD_DETACH notification calls. 
		processAttach();
		break;
	case DLL_PROCESS_DETACH:
		processDetach();
		break;
	}
	return TRUE;
}