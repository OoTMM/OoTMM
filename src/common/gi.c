#include <combo.h>

const GetItem kExtendedGetItems[] = {
#define X(a, b, c, d, e)    {a, b, Y(c), d, Z(e)}
#define Y(x)                x
#define Z(x)                x

#if defined(GAME_OOT)
# include "data/oot/gi.inc"
# undef Y
# undef Z
# define Y(x)               ((x) | 0x80)
# define Z(x)               ((x) | 0x1000)
# include "data/mm/gi.inc"
#endif

#undef X
#undef Y
#undef Z
};
