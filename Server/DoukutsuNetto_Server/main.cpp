#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma comment(linker, "/ENTRY:DllMain")

DWORD ServerHandler()
{
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwMsg, LPVOID lpReserved)
{
	switch(dwMsg)
    {
    case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ServerHandler, 0, NULL, NULL);
        return true;
    case DLL_PROCESS_DETACH:
		// running = false
		return true;
    }
	return true;
}