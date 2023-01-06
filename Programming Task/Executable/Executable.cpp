#include <windows.h>
#include <stdio.h>

int main() {

	// Show messagebox before hooking.
	MessageBoxA(NULL, "Morphisec", "Example", MB_OKCANCEL);


	// Load the specified module into the address space of the process.
	HINSTANCE hDll = LoadLibraryA("CustomDLL.dll");
	if (hDll == NULL)
	{
		printf("Failed to Load DLL file\n");
		return 1;
	}

	// Show messagebox after hooking.
	MessageBoxA(NULL, "Morphisec", "Example", MB_OKCANCEL);

	// Free the loaded dll module.
	FreeLibrary(hDll);


	return 0;
}