#include <combo.h>
#include <combo/custom.h>

const GetItem kExtendedGetItems[] = {
#define X(a, b, c, drawGiParam, d, e, text) {a, b, c, 8, e}
#if defined(GAME_OOT)
# define OOT(x)            x
# define MM(x)             (x ^ MASK_FOREIGN_OBJECT)
#else
# define OOT(x)            (x ^ MASK_FOREIGN_OBJECT)
# define MM(x)             x
#endif
#include "data/gi.inc"
#undef OOT
#undef MM
#undef X
};

/* Draw GI Params */
const u8 kGetItemDrawGiParam[] = {
#define X(a, b, c, drawGiParam, d, e, text) drawGiParam
#include "data/gi.inc"
#undef X
};


void comboLoadObjectGi(Actor_Player* player, u16 objectId)
{
    if (objectId & ~MASK_FOREIGN_OBJECT)
    {
        /* Unsure what this does */
        /* OoT: *((u8*)player + 0x160) = 1; */
        /* MM: *((u8*)player + 0x1b8) = 1; */

        osCreateMesgQueue(&player->objMsgQueue, &player->objMsg, 1);
        comboLoadObject(player->objBuffer, objectId);
        osSendMesg(&player->objMsgQueue, NULL, OS_MESG_NOBLOCK);
    }
}
