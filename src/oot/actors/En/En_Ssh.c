#include <combo.h>

static const s16 kNPCs[] = {
    NPC_OOT_GS_10,
    NPC_OOT_GS_20,
    NPC_OOT_GS_30,
    NPC_OOT_GS_40,
    NPC_OOT_GS_50,
};

int EnSsh_TalkedTo(Actor* this, GameState_Play* play)
{
    static int sInMessage = 0;

    s16 gi;
    s16 npc;
    u16 itemId;

    if (this->variable == 0)
        return ActorTalkedTo(this);
    if (sInMessage)
    {
        sInMessage = 0;
        npc = kNPCs[this->variable - 1];
        gi = comboOverrideEx(OV_NPC, 0, npc, GI_OOT_RUPEE_BLUE, 0);
        itemId = comboItemFromGI(gi);
        comboTextHijackSkullReward(play, itemId, this->variable * 10);
        return 1;
    }
    if (ActorTalkedTo(this))
    {
        sInMessage = 1;
        return 0;
    }
    return 0;
}

PATCH_CALL(0x80b26d58, EnSsh_TalkedTo);
