#include <combo.h>

int EnSyatekiMan_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    int npcQuiver;
    int npcHeartPiece;

    if (play->sceneId == SCE_MM_SHOOTING_GALLERY)
    {
        npcQuiver = NPC_MM_SHOOTING_GAME_TOWN_1;
        npcHeartPiece = NPC_MM_SHOOTING_GAME_TOWN_2;
    }
    else
    {
        npcQuiver = NPC_MM_SHOOTING_GAME_SWAMP_1;
        npcHeartPiece = NPC_MM_SHOOTING_GAME_SWAMP_2;
    }

    switch (gi)
    {
    case GI_MM_BOW:
    case GI_MM_QUIVER2:
    case GI_MM_QUIVER3:
        gi = comboOverride(OV_NPC, 0, npcQuiver, gi);
        break;
    case GI_MM_HEART_PIECE:
        gi = comboOverride(OV_NPC, 0, npcHeartPiece, gi);
        break;
    }
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x809c7b64, EnSyatekiMan_GiveItem);
PATCH_CALL(0x809c7b90, EnSyatekiMan_GiveItem);
PATCH_CALL(0x809c7bc4, EnSyatekiMan_GiveItem);
PATCH_CALL(0x809c7bdc, EnSyatekiMan_GiveItem);
PATCH_CALL(0x809c7e14, EnSyatekiMan_GiveItem);
PATCH_CALL(0x809c7e30, EnSyatekiMan_GiveItem);
PATCH_CALL(0x809c7e64, EnSyatekiMan_GiveItem);
PATCH_CALL(0x809c7e7c, EnSyatekiMan_GiveItem);
