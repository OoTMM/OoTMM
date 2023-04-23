#include <combo.h>

static const s16 kNPCs[] = {
    NPC_OOT_GS_10,
    NPC_OOT_GS_20,
    NPC_OOT_GS_30,
    NPC_OOT_GS_40,
    NPC_OOT_GS_50,
};

void EnSsh_TalkedTo(Actor* this, GameState_Play* play)
{
    s16 gi;
    s16 npc;

    /* Big skulltula */
    if (this->variable == 0)
        return;

    npc = kNPCs[this->variable - 1];
    gi = comboOverrideEx(OV_NPC, 0, npc, GI_OOT_RUPEE_BLUE, 0);
    comboTextHijackSkullReward(play, gi, this->variable * 10);
}
