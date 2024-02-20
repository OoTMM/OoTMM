#include <combo.h>
#include <combo/souls.h>

static int ObjNozoki_CanOpenDoorForKafeiWrapper(Actor* actor, GameState_Play* play)
{
    int (*ObjNozoki_CanOpenDoorForKafei)(Actor*, GameState_Play*);
    int checkMsgBox;

    /* We need to check for souls explicitely here, as this doesn't use the normal flags */
    checkMsgBox = 0;
    switch (actor->variable & 0x7f)
    {
    case 0x42:
    case 0x43:
        if (!comboHasSoulMm(GI_MM_SOUL_ENEMY_DEKU_BABA))
            return 0;
        checkMsgBox = 1;
        break;
    case 0x44:
        if (!comboHasSoulMm(GI_MM_SOUL_ENEMY_WOLFOS))
            return 0;
        checkMsgBox = 1;
        break;
    }

    if (checkMsgBox)
    {
        if (Message_GetState(&play->msgCtx) != 0)
            return 0;
    }

    ObjNozoki_CanOpenDoorForKafei = actorAddr(0x233, 0x80ba2708);
    return ObjNozoki_CanOpenDoorForKafei(actor, play);
}

PATCH_CALL(0x80ba2804, ObjNozoki_CanOpenDoorForKafeiWrapper);
