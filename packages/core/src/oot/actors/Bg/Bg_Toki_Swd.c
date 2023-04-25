#include <combo.h>

static void BgTokiSwd_GiveItem(Actor* this, GameState_Play* play, s16 gi, int npc, int event)
{
    if (Actor_HasParent(this))
    {
        this->attachedA = NULL;
        SetEventChk(event);
    }
    else
    {
        gi = comboOverride(OV_NPC, 0, npc, gi);
        GiveItem(this, play, gi, 200.f, 50.f);
    }
}

static void swapFarore(void)
{
    OotFaroreWind* ptr;
    OotFaroreWind tmp;

    ptr = &gOotSave.fw;
    memcpy(&tmp, &ptr[0], sizeof(tmp));
    memcpy(&ptr[0], &ptr[-1], sizeof(tmp));
    memcpy(&ptr[-1], &tmp, sizeof(tmp));
}

void BgTokiSwd_Handler(Actor* this, GameState_Play* play)
{
    TransitionContext* t;

    if (gSave.age == AGE_CHILD && !GetEventChk(EV_OOT_CHK_MASTER_SWORD_CHAMBER))
    {
        BgTokiSwd_GiveItem(this, play, GI_OOT_SWORD_MASTER, NPC_OOT_MASTER_SWORD, EV_OOT_CHK_MASTER_SWORD_CHAMBER);
        return;
    }

    if (gSave.age == AGE_ADULT && !GetEventChk(EV_OOT_CHK_MASTER_SWORD_PULLED))
    {
        BgTokiSwd_GiveItem(this, play, GI_OOT_MEDALLION_LIGHT, NPC_OOT_LIGHT_MEDALLION, EV_OOT_CHK_MASTER_SWORD_PULLED);
        return;
    }

    if (Actor_HasParent(this))
    {
        /* Swap farore */
        swapFarore();

        /* Time Travel */
        t = &play->transition;
        t->type = TRANS_TYPE_NORMAL;
        t->gfx = TRANS_GFX_SHORTCUT;
        t->entrance = 0x02ca;
        t->age = !(gSave.age);
    }
    else
    {
        /* Needs the Master Sword to become adult */
        if (gSave.inventory.equipment.swords & EQ_OOT_SWORD_MASTER)
            ActorEnableGrab(this, play);
    }
}

PATCH_FUNC(0x8091902c, BgTokiSwd_Handler);
