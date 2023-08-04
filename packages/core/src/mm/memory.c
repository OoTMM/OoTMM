#include <combo.h>

extern u32 gMemorySize;
void ClearMemoryRange(void* start, void* end);

void ClearHiMemory(void)
{
    /* TODO: Clean this up (a lot) */
    ClearMemoryRange((void*)0x80208ea0, (void*)PAYLOAD_RAM - 0x30000);
    ClearMemoryRange((void*)(PAYLOAD_RAM + PAYLOAD_SIZE), (void*)gMemorySize);
}
