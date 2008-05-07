#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock.h>
#include <stdio.h>

#define WSCK_ERROR -1
#define WSCK_OK     0

#pragma comment(linker, "/ENTRY:DllMain")

bool poll_exit = false;

// This will eventually get its own file
class Buffer
{
public:
	Buffer(int size);
	~Buffer();

	void Resize(int len);

	void WriteByte(BYTE val);
	void WriteShort(short val);
	void WriteInt(int val);
	//void WriteLong(long val);// UNUSED
	void WriteString(char* val);
	void WriteShortLengthString(char* val);

	BYTE* GetBytes() {return BufferData;};

private:
	BYTE *BufferData;
	int Index;
	int Length;
};

Buffer::Buffer(int size)
{
	BufferData = new BYTE[size];
	Index = 0;
	Length = size;
}
Buffer::~Buffer()
{
	delete[] BufferData;
	Index = 0;
}

// **NOTE: Resize will keep old index unless new length is less than old length
void Buffer::Resize(int len)
{
	BYTE *NewBuffer = new BYTE[len];

	for (int iter = 0; iter < Length; ++iter)
	{
		NewBuffer[iter] = BufferData[iter];
	}

	// delete old buffer, remake it, copy new buffer to old buffer, and delete new buffer
	delete[] BufferData;
	BufferData = new BYTE[len];
	memcpy(BufferData, NewBuffer, len);
	delete[] NewBuffer;

	if (Index > len)
		Index = len;

	Length = len;
}

void Buffer::WriteByte(BYTE val)
{
	BufferData[Index] = val;
	++Index;
	++Length;
}
void Buffer::WriteShort(short val)
{
	if (Index + 2 > Length)
		Resize(Index - Length + 2);

	BufferData[Index] = (BYTE)val;
	++Index;
	BufferData[Index] = (BYTE)(val / 256);
	++Index;

	Length += 2;
}
void Buffer::WriteInt(int val)
{
	if (Index + 4 > Length)
		Resize(Index - Length + 4);

	BufferData[Index] = (BYTE)val;
	++Index;
	BufferData[Index] = (BYTE)(val / 256);
	++Index;
	BufferData[Index] = (BYTE)(val / 65536);
	++Index;
	BufferData[Index] = (BYTE)(val / 16777216);
	++Index;

	Index += 4;
	Length += 4;
}
void Buffer::WriteString(char* val)
{
	if (Index + sizeof(val) > Length)
		Resize(Index - Length + sizeof(val));

	for (int iter = 0; iter < sizeof(val); ++iter)
	{
		BufferData[Index] = val[Index];
		++Index;
	}

	Length += sizeof(val);
}
// This is different from WriteString in the fact that it writes the string's length
// before writing the string
void Buffer::WriteShortLengthString(char* val)
{
	if (Index + sizeof(val) > Length)
		Resize(Index - Length + sizeof(val));
	
	WriteShort(sizeof(val));
	for (int iter = 0; iter < sizeof(val); ++iter)
	{
		BufferData[Index] = val[Index];
		++Index;
	}

	Length += sizeof(val);
}

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