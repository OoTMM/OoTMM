#ifndef COMBO_ENEMY_KEEP_GBI_H
#define COMBO_ENEMY_KEEP_GBI_H

extern u8 FOREIGN_ENEMY_API_SEGMENTED_TO_VIRTUAL[];
static inline void* enemyPortKeepPointer(const void* pointer)
{
    u32 segment = (u32)pointer >> 24;
    if (segment == 4 || segment == 5)
        return ((void* (*)(void*))(void*)FOREIGN_ENEMY_API_SEGMENTED_TO_VIRTUAL)((void*)pointer);
    return (void*)pointer;
}

static inline void enemyPortKeepDisplayList(Gfx* packet, const void* pointer)
{
    gSPDisplayList(packet, enemyPortKeepPointer(pointer));
}

static inline void enemyPortKeepSegment(Gfx* packet, u32 segment, const void* pointer)
{
    gSPSegment(packet, segment, enemyPortKeepPointer(pointer));
}

static inline void enemyPortKeepTexture(Gfx* packet, u32 format, u32 size, u32 width, const void* pointer)
{
    gDPSetTextureImage(packet, format, size, width, enemyPortKeepPointer(pointer));
}

#undef gSPDisplayList
#define gSPDisplayList(packet, pointer) enemyPortKeepDisplayList((packet), (pointer))
#undef gSPSegment
#define gSPSegment(packet, segment, pointer) enemyPortKeepSegment((packet), (segment), (pointer))
#undef gDPSetTextureImage
#define gDPSetTextureImage(packet, format, size, width, pointer) enemyPortKeepTexture((packet), (format), (size), (width), (pointer))
#undef SEGMENTED_TO_VIRTUAL
#define SEGMENTED_TO_VIRTUAL(pointer) ((void* (*)(void*))(void*)FOREIGN_ENEMY_API_SEGMENTED_TO_VIRTUAL)((void*)(pointer))

#endif
