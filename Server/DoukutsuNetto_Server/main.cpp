#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock.h>
#include <stdio.h>

#include "buffer.h"

#define WSCK_ERROR -1
#define WSCK_OK     0

#pragma comment(linker, "/ENTRY:DllMain")

bool poll_exit = false;

DWORD ServerHandler()
{
	bool running = true;

	// player X and Y, 4 bytes at 0049E654 and 0049E658
	DWORD *playerX = (DWORD*)0x0049E654;
	DWORD *playerY = (DWORD*)0x0049E658;

	while (running)
	{
		// Ctrl + F12 unloads the DLL
		if (GetKeyState(VK_CONTROL) && GetKeyState(VK_F12))
			running = false;

		// If polled to exit then stop running
		if (poll_exit)
			running = false;

		// Sleep to allow execution of other threads
		Sleep(10);
	}

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
		poll_exit = true;
		return true;
    }
	return true;
}