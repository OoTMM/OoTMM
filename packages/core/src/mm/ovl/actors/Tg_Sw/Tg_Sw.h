#ifndef OVL_TG_SW
#define OVL_TG_SW

#include <combo.h>

struct TGSw;
typedef void (*TGSwActionFunc)(struct TGSw*, PlayState*);

#define TGSW_GET_FC(thisx) ((((thisx)->params & 0xFC) >> 2) & 0xFF)

#define ENSW_GETS_FC(params) (((params & 0xFC) >> 2) & 0xFF)
#define ENSW_GET_FC(thisx) (ENSW_GETS_FC((thisx)->params))

typedef struct TGSw {
    /* 0x000 */ Actor actor;
    /* 0x144 */ TGSwActionFunc actionFunc;
} TGSw; // size = 0x148

#endif // Z_TG_SW_H
