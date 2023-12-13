#include <combo.h>
#include <combo/custom.h>

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
