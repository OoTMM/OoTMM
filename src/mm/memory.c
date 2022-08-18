#include <combo.h>

extern u32 gMemorySize;
void ClearMemoryRange(void* start, void* end);

void ClearHiMemory(void)
{
    ClearMemoryRange((void*)0x80208ea0, (void*)PAYLOAD_RAM);
    ClearMemoryRange((void*)(PAYLOAD_RAM + PAYLOAD_SIZE), (void*)gMemorySize);
}
