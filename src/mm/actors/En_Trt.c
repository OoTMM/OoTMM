#include <combo.h>

int EnTrt_GiveItem_BottledRedPotion(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    gi = comboOverride(OV_NPC, 0, NPC_MM_KOTAKE_RED_POTION, gi);
    return GiveItem(actor, play, gi, a, b);
}

PATCH_CALL(0x80a8c54c, EnTrt_GiveItem_BottledRedPotion);
PATCH_CALL(0x80ad4094, EnTrt_GiveItem_BottledRedPotion); /* En_Trt2 */
