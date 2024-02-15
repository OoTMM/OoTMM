#include <combo.h>
#include <combo/souls.h>

static int ObjNozoki_CanOpenDoorForKafeiWrapper(Actor* actor, GameState_Play* play)
{
    int (*ObjNozoki_CanOpenDoorForKafei)(Actor*, GameState_Play*);

    /* We need to check for souls explicitely here, as this doesn't use the normal flags */
    switch (actor->variable & 0x7f)
    {
    case 0x42:
    case 0x43:
        if (!comboHasSoulMm(GI_MM_SOUL_ENEMY_DEKU_BABA))
            return 0;
        break;
    case 0x44:
        if (!comboHasSoulMm(GI_MM_SOUL_ENEMY_WOLFOS))
            return 0;
        break;
    }

    ObjNozoki_CanOpenDoorForKafei = actorAddr(0x233, 0x80ba2708);
    return ObjNozoki_CanOpenDoorForKafei(actor, play);
}

PATCH_CALL(0x80ba2804, ObjNozoki_CanOpenDoorForKafeiWrapper);
