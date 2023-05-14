#include <combo.h>

void EnGs_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    if (gi == GI_MM_HEART_PIECE)
    {
        gi = comboOverride(OV_NPC, 0, NPC_MM_GOSSIP_HEART_PIECE, gi);
    }
    GiveItem(this, play, gi, a, b);
}

PATCH_CALL(0x809989cc, EnGs_GiveItem);
PATCH_CALL(0x80998a30, EnGs_GiveItem);

void EnGs_MessageBox(GameState_Play* play, Actor* this)
{
    u16 tmp;
    u8 key;

    if (this->variable == 1 || this->variable == 2)
    {
        tmp = *(u16*)((char*)this + 0x198);
        key = (tmp & 0x1f) | 0x20;
    }
    else
    {
        key = (*(u8*)((char*)this + 0x195) & 0x1f);
        switch (play->sceneId)
        {
        case SCE_MM_MOON_DEKU:
        case SCE_MM_MOON_GORON:
        case SCE_MM_MOON_ZORA:
        case SCE_MM_MOON_LINK:
            key |= 0x40;
            break;
        }
    }
    PlayerDisplayTextBox(play, 0x20d0, this);
    comboHintGossip(key, play);
}

PATCH_CALL(0x80997e98, EnGs_MessageBox);
