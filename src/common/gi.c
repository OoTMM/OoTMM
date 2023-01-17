#include <combo.h>
#include <combo/custom.h>

const GetItem kExtendedGetItems[] = {
#define X(a, b, c, d, e)    {a, b, c, 8, Y(e)}
#define Y(x)                x

#if defined(GAME_OOT)
# include "data/oot/gi.inc"
#else
# include "data/mm/gi.inc"
#endif
# undef Y
# define Y(x)               ((((x) < 0x2000) * ((x) | MASK_FOREIGN_OBJECT)) | ((x) >= 0x2000) * (x))
#if defined(GAME_OOT)
# include "data/mm/gi.inc"
#else
# include "data/oot/gi.inc"
#endif

#undef X
#undef Y
};

u16 comboItemFromGI(s32 gi)
{
    u16 itemId;

    itemId = kExtendedGetItems[gi - 1].itemId;
    if (gi & MASK_FOREIGN_GI)
        itemId |= MASK_FOREIGN_ITEM;
    return itemId;
}

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
