#include <combo.h>
#include <combo/custom.h>

const GetItem kExtendedGetItems[] = {
#define X(a, b, c, d, e)    {a, b, Y(c), 8, Z(e)}
#define Y(x)                x
#define Z(x)                x

#if defined(GAME_OOT)
# include "data/oot/gi.inc"
#else
# include "data/mm/gi.inc"
#endif
# undef Y
# undef Z
# define Y(x)               ((x) | 0x80)
# define Z(x)               ((x) | 0x1000)
#if defined(GAME_OOT)
# include "data/mm/gi.inc"
#else
# include "data/oot/gi.inc"
#endif

#undef X
#undef Y
#undef Z
};

void comboLoadObjectGi(Actor_Player* player, u16 objectId)
{
    if (objectId)
    {
        /* Unsure what this does */
        /* OoT: *((u8*)player + 0x160) = 1; */
        /* MM: *((u8*)player + 0x1b8) = 1; */

        osCreateMesgQueue(&player->objMsgQueue, &player->objMsg, 1);
        comboLoadObject(player->objBuffer, objectId);
        osSendMesg(&player->objMsgQueue, NULL, OS_MESG_NOBLOCK);
    }
}
