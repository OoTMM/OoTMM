#include "sys_cfb.h"

#include "libu64/debug.h"
#include "attributes.h"
#include "line_numbers.h"
#include "global.h"

#pragma increment_block_number "gc-eu:128 gc-eu-mq:128 gc-jp:128 gc-jp-ce:128 gc-jp-mq:128 gc-us:128 gc-us-mq:128" \
                               "pal-1.1:128"

uintptr_t sSysCfbFbPtr[2];
uintptr_t sSysCfbEnd;

void SysCfb_Init(s32 n64dd) {
    u32 screenSize;
    (void)n64dd;

    screenSize = SCREEN_WIDTH * SCREEN_HEIGHT;
    sSysCfbEnd = 0x80800000;

    sSysCfbFbPtr[0] = sSysCfbEnd - (screenSize * 4);
    sSysCfbFbPtr[1] = sSysCfbEnd - (screenSize * 2);
}

void SysCfb_Reset(void) {
    sSysCfbFbPtr[0] = 0;
    sSysCfbFbPtr[1] = 0;
    sSysCfbEnd = 0;
}

void* SysCfb_GetFbPtr(s32 idx) {
    if (idx < 2) {
        return (void*)sSysCfbFbPtr[idx];
    }
    return NULL;
}

void* SysCfb_GetFbEnd(void) {
    return (void*)sSysCfbEnd;
}
