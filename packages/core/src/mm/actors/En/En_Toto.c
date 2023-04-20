#include <combo.h>

void EnToto_SpawnItemGiver(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    comboSpawnItemGiver(play, NPC_MM_MASK_TROUPE_LEADER);
    this->attachedA = &GET_LINK(play)->base;
}

PATCH_CALL(0x80ba4e10, EnToto_SpawnItemGiver);
