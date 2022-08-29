#include <combo.h>

void hook_LoadObjectGi(Actor_Player* player, u16 objectId)
{
    if (objectId)
    {
        /* Unsure what this does */
        /* *((u8*)player + 0x160) = 1; */

        osCreateMesgQueue(&player->objMsgQueue, &player->objMsg, 1);
        comboLoadObject(player->objBuffer, objectId);
        osSendMesg(&player->objMsgQueue, NULL, OS_MESG_NOBLOCK);
    }
}
