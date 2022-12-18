#include <combo.h>

typedef struct PACKED ALIGNED(2)
{
    u8 key;
    u8 type;
    u8 region;
    u8 unused;
    s16 item;
    s16 item2;
}
Hint;
