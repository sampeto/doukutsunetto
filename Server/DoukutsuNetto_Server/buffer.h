#include <windows.h>

#ifndef HG_BUFFER_H
#define HG_BUFFER_H

// provides for easy packet building and reading
class Buffer
{
public:
	// constructor, creates buffer with size
	Buffer(int size);
	// destructor frees memory
	~Buffer();

	// resizes buffer
	void Resize(int len);

	// writes a byte to buffer
	void WriteByte(BYTE val);
	// writes a short to buffer
	void WriteShort(short val);
	//writes an integer to buffer
	void WriteInt(int val);
	//void WriteLong(long val);// UNUSED
	// writes a string to buffer
	void WriteString(char* val);
	// writes a string preceded by a short telling the length of the string to buffer
	void WriteShortLengthString(char* val);

	// reads a byte from current index
	BYTE ReadByte();
	// reads a short from current index
	short ReadShort();
	// reads an integer from current index
	int ReadInt();
	// reads a string from current index
	char* ReadString(int len);
	// reads a string from current index using the preceding short as its length
	char* ReadShortLengthString();

	// gets bytes in buffer
	BYTE* GetBytes() {return BufferData;};

private:
	// bytes in buffer
	BYTE *BufferData;
	// current position in buffer
	int Index;
	// length of buffer
	int Length;
};

#endif