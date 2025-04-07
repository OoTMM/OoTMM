#ifndef INCLUDED_COMBO_ALLOC_H
#define INCLUDED_COMBO_ALLOC_H

#include <PR/ultratypes.h>

/* Custom Heap */
void    CustomHeap_Init(void);
void*   CustomHeap_Alloc(size_t size);
void    CustomHeap_Free(void* data);
void*   CustomHeap_Realloc(void* data, size_t size);

#endif
