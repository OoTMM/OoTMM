#include <combo.h>
#include <combo/custom.h>

const ExtendedItemTable kExtendedGetItemsTable = {
#define X(a, b, c, d, e, text) {a, b, c, 8, Y(e)}
#define Y(x)                x

{
#if defined(GAME_OOT)
# include "data/oot/gi.inc"
#else
# include "data/mm/gi.inc"
#endif
},

# undef Y
# define Y(x)               ((((x) < 0x2000) * ((x) | MASK_FOREIGN_OBJECT)) | ((x) >= 0x2000) * (x))

{
#if defined(GAME_OOT)
# include "data/mm/gi.inc"
#else
# include "data/oot/gi.inc"
#endif
},

#undef X
#undef Y
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
