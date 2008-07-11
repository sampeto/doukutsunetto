#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock.h>
#include <stdio.h>

#include "buffer.h"

#define WSCK_V1            0x0101
#define WSCK_V2            0x0202
#define WSCK_ERROR -1
#define WSCK_OK     0

#define ERR_STARTUP			0x01
#define ERR_WRONGVERSION	0x02
#define	ERR_SOCKETERR		0x04
#define ERR_BINDERR			0x08

#pragma comment(linker, "/ENTRY:DllMain")

enum PACKETHEADER {
	ID_LOC	=	0x10,	// location of player and map ID
};


bool poll_exit = false;

DWORD errorFlags = 0;

DWORD ServerHandler()
{

	bool running = true;

	// Socket handler
	SOCKET socketHnd;
	// Winsock data
	WSADATA wsaData;
	
	// Check for error
	if (WSAStartup(WSCK_V2, &wsaData))
	{
		errorFlags &= ERR_STARTUP;
		running = false;
	}

	// We want version 2
	if (wsaData.wVersion != WSCK_V2)
	{
		errorFlags &= ERR_WRONGVERSION;
		running = false;
	}
	
	// For TCP...
	SOCKADDR_IN sckAddr;
	sckAddr.sin_family = AF_INET;
	sckAddr.sin_port = htons(9009);	// Port 9009, will probably change to load from a config file later
	sckAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Listen from connections from ANY computer 

	// Create the socket
	socketHnd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socketHnd == INVALID_SOCKET)
    {
        errorFlags &= ERR_SOCKETERR;
		running = false;
    }

	if (bind(socketHnd, (LPSOCKADDR)&sckAddr, sizeof(sckAddr)) == SOCKET_ERROR)
    {
		errorFlags &= ERR_BINDERR;
        running = false;
    }

	if (running && errorFlags == 0)
	{
		MessageBoxA(NULL, "Server initialized, close this window to begin listening!", "Info", MB_ICONINFORMATION);
		// Listen with one backlog max
		listen(socketHnd, 1);
	}

	// player X and Y, 4 bytes at 0049E654 and 0049E658
	DWORD *playerX;
	DWORD *playerY;

	DWORD *playerMapID;

	PACKETHEADER PID;
	while (running)
	{
		playerX = (DWORD*)0x0049E654;
		playerY = (DWORD*)0x0049E658;

		playerMapID = (DWORD*)0x004A57F0;

		// Ctrl + F12 unloads the DLL
		if (GetKeyState(VK_CONTROL) && GetKeyState(VK_F12))
			running = false;

		// If polled to exit then stop running
		if (poll_exit)
			running = false;

		/*=========================================
		/* Data sending
		/*=========================================*/
		Buffer buf(13);	// 13 bytes in length
		buf.WriteByte(ID_LOC);
		buf.WriteInt((int)&playerX);
		buf.WriteInt((int)&playerY);
		buf.WriteInt((int)&playerMapID);

		send(socketHnd, (char*)buf.GetBytes(), buf.GetLength(), 0);
		//buf.Clear();

		Buffer buf2(80);
		// Pass the old buffer pointer
		recv(socketHnd, (char*)buf2.GetBytes(), sizeof(buf2.GetBytes())-1, 0); 

		// Sleep to allow execution of other threads (and limit actions to about 30 FPS)
		Sleep(34);
	}

	//MessageBoxA(NULL, "test", "test", NULL);

	WSACleanup();

	//if (errorFlags > 0)
	//{
	//	char errMsg[1024];
	//	strcpy(errMsg, "There were error(s):\n");

	//	if (errorFlags & ERR_STARTUP)
	//		strcat(errMsg, "An error occurred while WinSock was initializing\n");
	//	if (errorFlags & ERR_WRONGVERSION)
	//		strcat(errMsg, "WinSock initialized the wrong version\n");
	//	if (errorFlags & ERR_SOCKETERR)
	//		strcat(errMsg, "Socket creation failed\n");
	//	if (errorFlags & ERR_BINDERR)
	//		strcat(errMsg, "Socket bind failed\n");
	//	
	//	MessageBoxA(NULL, errMsg, "Error", MB_ICONERROR);
	//}

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