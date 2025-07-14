#include "memory.h"

void WriteMemoryFloat(uintptr_t address, float value)
{
	mprotect((void*)(address & 0xFFFFF000), PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
	unsigned char const * p = reinterpret_cast<unsigned char const *>(&value);
	memcpy((void*)address, (void*)p, sizeof(float));
}

void WriteMemoryInt(uintptr_t address, int value)
{
	mprotect((void*)(address & 0xFFFFF000), PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
	unsigned char const * p = reinterpret_cast<unsigned char const *>(&value);
	memcpy((void*)address, (void*)p, sizeof(int));
}
