#pragma once

#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

void WriteMemoryFloat(uintptr_t address, float value);
void WriteMemoryInt(uintptr_t address, int value);
void WriteMemoryByte(uintptr_t address, char value);

