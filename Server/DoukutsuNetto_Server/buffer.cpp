#include "buffer.h"

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
	Length = 0;
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

BYTE Buffer::ReadByte()
{
	BYTE read = BufferData[Index];
	++Index;
	return read;
}

short Buffer::ReadShort()
{
	short read = (short)((BufferData[Index]) + (BufferData[Index + 1] * 256));
	++Index;
	return read;
}

int Buffer::ReadInt()
{
	int read = (BufferData[Index]) + (BufferData[Index + 1] * 256) + (BufferData[Index + 2] * 65536) + (BufferData[Index + 3] * 16777216);
    Index += 4;
    return read;
}

char* Buffer::ReadString(int len)
{
	//...?... not sure about this...
	// dangerous code is dangerous, make sure to delete result
	char* read = new char[len];
	memcpy(read, BufferData + Index, len);
	Index += len;
	return read;
}

char* Buffer::ReadShortLengthString()
{
	// DON'T TRY THIS AT HOME
	short len = ReadShort();
	return ReadString(len);
}