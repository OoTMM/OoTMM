#include <combo.h>
#include <combo/item.h>
#include <combo/draw.h>

int Fishing_HasGivenItem(Actor* this)
{
    if (Actor_HasParentZ(this))
    {
        if (gSave.age == AGE_CHILD)
            gOotExtraFlags.fishingChild = 1;
        else
            gOotExtraFlags.fishingAdult = 1;
        return 1;
    }
    return 0;
}

PATCH_CALL(0x80a427ac, Fishing_HasGivenItem);

void Fishing_GiveItem(Actor* this, PlayState* play, s16 gi, float a, float b)
{
    int npc;

    npc = -1;
    if (gSave.age == AGE_CHILD)
    {
        if (gOotExtraFlags.fishingChild)
            gi = GI_OOT_RUPEE_BLUE;
        else
        {
            npc = NPC_OOT_FISH_CHILD;
            gi = GI_OOT_HEART_PIECE;
        }
    }
    else
    {
        if (gOotExtraFlags.fishingAdult)
            gi = GI_OOT_RUPEE_BLUE;
        else
        {
            npc = NPC_OOT_FISH_ADULT;
            gi = GI_OOT_SCALE_SILVER;
        }
    }

    comboGiveItemNpc(this, play, gi, npc, a, b);
}

PATCH_CALL(0x80a42644, Fishing_GiveItem);
PATCH_CALL(0x80a427e0, Fishing_GiveItem);

static void Fishing_Fish_ItemQuery(PlayState* play, ComboItemQuery* q, u16 variable)
{
    int flag;

    flag = variable - 100;
    if (gSave.age == AGE_ADULT)
        flag += 17;

    bzero(q, sizeof(*q));
    q->gi = gSave.age == AGE_ADULT
        ? GI_OOT_FISHING_POND_CHILD_FISH_2LBS
        : GI_OOT_FISHING_POND_ADULT_FISH_4LBS;
    q->ovType = OV_FISH;
    q->id = flag; /* EN_FISH_PARAM */
    q->giRenew = GI_OOT_RECOVERY_HEART;

    if (BITMAP8_GET(gSharedCustomSave.caughtFishFlags, flag))
        q->ovFlags |= OVF_RENEW;
}

static void Fishing_Fish_ItemOverride(PlayState* playOrNull, ComboItemOverride* o, u16 variable)
{
    ComboItemQuery q;

    Fishing_Fish_ItemQuery(playOrNull, &q, variable);
    comboItemOverride(o, &q);
}


void Fishing_DrawFish_SkelAnime(PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount,
                           void* overrideLimbDraw, void* postLimbDraw, Actor* this) {
    ComboItemOverride o;
    s16 gi;

    Fishing_Fish_ItemOverride(play, &o, this->params);
    if (o.gi >= GI_OOT_FISHING_POND_CHILD_FISH_2LBS && o.gi <= GI_OOT_FISHING_POND_ADULT_LOACH_36LBS)
    {
        SkelAnime_DrawFlexOpa(play, skeleton, jointTable, dListCount, overrideLimbDraw, postLimbDraw, this);
        return;
    }

    Vec3f mouthOffset = { 0.0f, 0.0f, 0.0f };
    Vec3f* fishMouthPos = (Vec3f*)(((u8*)this) + 0x1B0);
    Matrix_MultVec3f(&mouthOffset, fishMouthPos);
    Matrix_Scale(56.0f, 56.0f, 56.0f, MTXMODE_APPLY);
    gi = o.gi;
    if (o.cloakGi)
        gi = o.cloakGi;
    Draw_Gi(play, this, gi, 0);
}

PATCH_CALL(0x80a405d8, Fishing_DrawFish_SkelAnime);
PATCH_CALL(0x80a4066c, Fishing_DrawFish_SkelAnime);

static f32 Fishing_GetFishOnHand(u8* sFishOnHandIsLoach, s32 take) {
    u8 pounds = 0;
    if (gSave.age == AGE_ADULT) {
        if (gSharedCustomSave.caughtAdultFishWeight[0]) {
            pounds = gSharedCustomSave.caughtAdultFishWeight[gSharedCustomSave.caughtAdultFishWeight[0]];
            if (take) {
                gSharedCustomSave.caughtAdultFishWeight[0]--;
            }
        }
    } else {
        if (gSharedCustomSave.caughtChildFishWeight[0]) {
            pounds = gSharedCustomSave.caughtChildFishWeight[gSharedCustomSave.caughtChildFishWeight[0]];
            if (take) {
                gSharedCustomSave.caughtChildFishWeight[0]--;
            }
        }
    }

    if (pounds) {
        if (pounds & 0x80) {
            /* Loach */
            *sFishOnHandIsLoach = 1;
        } else {
            /* Fish */
            *sFishOnHandIsLoach = 0;
        }
        pounds &= 0x7F;
        return FISH_WEIGHT_TO_LENGTH(pounds);
    }
    return 0;
}

f32 Fishing_CheckFishOnHand(u8* sFishOnHandIsLoach) {
    return Fishing_GetFishOnHand(sFishOnHandIsLoach, 0);
}

f32 Fishing_TakeFishOnHand(u8* sFishOnHandIsLoach) {
    return Fishing_GetFishOnHand(sFishOnHandIsLoach, 1);
}

Actor* Fishing_DiscardFish(Actor* this) {
    u8 fishOnHandIsLoach;
    Fishing_GetFishOnHand(&fishOnHandIsLoach, 1);
    s16* stateAndTimer = (s16*)(((u8*)this) + 0x14C);

    /* Displaced code: */
    *stateAndTimer = 20;

    return this;
}

void Fishing_SetCaughtId(Actor* actor, u16* sFishingCaughtTextId) {
    /* Displaced code: */
    u8* stateAndTimer = ((u8*)actor) + 0x1C5;
    *stateAndTimer = 0;

    *sFishingCaughtTextId = actor->params;
}

void Fishing_HijackCaughtMessage(PlayState* play, u16 textId, Actor* this) {
    if (textId >= 200) {
        PlayerDisplayTextBox(play, textId, this);
        return;
    }

    u16 variable = textId;
    textId = 0x4083; /* standard fish caught */

    PlayerDisplayTextBox(play, textId, this);

    ComboItemOverride o;

    Fishing_Fish_ItemOverride(play, &o, variable);
    comboTextHijackFishCaught(play, &o);
}

PATCH_CALL(0x80a42b98, Fishing_HijackCaughtMessage);

s32 Fishing_IsFishLoach(u16 variable) {
    if (variable == 200) {
        /* Fish Tank */
        return 0;
    }

    ComboItemOverride o;

    Fishing_Fish_ItemOverride(NULL, &o, variable);

    if (o.gi >= GI_OOT_FISHING_POND_CHILD_FISH_2LBS && o.gi <= GI_OOT_FISHING_POND_ADULT_FISH_25LBS)
    {
        return 0;
    }

    if (o.gi >= GI_OOT_FISHING_POND_CHILD_LOACH_14LBS && o.gi <= GI_OOT_FISHING_POND_ADULT_LOACH_36LBS)
    {
        return 1;
    }

    return variable >= 115;
}

void Fishing_OverrideInitFishLength(u8 linkAge, f32 childMultiplier, Actor* this) {
    f32* fishLength = (f32*)(((u8*)this)+0x19C);
    u8* appearsAsLoach = (((u8*)this)+0x143); /* unused padding in Fishing struct */

    /* Displaced code: */
    if (linkAge == AGE_CHILD) {
        *fishLength *= childMultiplier;
    }

    ComboItemOverride o;

    Fishing_Fish_ItemOverride(NULL, &o, this->params);

    u8 pounds;

    if (o.gi >= GI_OOT_FISHING_POND_CHILD_FISH_2LBS && o.gi <= GI_OOT_FISHING_POND_CHILD_FISH_14LBS)
    {
        pounds = o.gi - GI_OOT_FISHING_POND_CHILD_FISH_2LBS + 2;
    }
    else if (o.gi >= GI_OOT_FISHING_POND_ADULT_FISH_4LBS && o.gi <= GI_OOT_FISHING_POND_ADULT_FISH_25LBS)
    {
        pounds = o.gi - GI_OOT_FISHING_POND_ADULT_FISH_4LBS + 4;
    }
    else if (o.gi >= GI_OOT_FISHING_POND_CHILD_LOACH_14LBS && o.gi <= GI_OOT_FISHING_POND_CHILD_LOACH_19LBS)
    {
        pounds = o.gi - GI_OOT_FISHING_POND_CHILD_LOACH_14LBS + 14;
        *appearsAsLoach = 1;
    }
    else if (o.gi >= GI_OOT_FISHING_POND_ADULT_LOACH_29LBS && o.gi <= GI_OOT_FISHING_POND_ADULT_LOACH_36LBS)
    {
        pounds = o.gi - GI_OOT_FISHING_POND_ADULT_LOACH_29LBS + 29;
        *appearsAsLoach = 1;
    }
    else
    {
        pounds = 0;
    }

    if (pounds)
    {
        *fishLength = FISH_WEIGHT_TO_LENGTH(pounds);
        if (*appearsAsLoach) {
            *fishLength *= 0.5f;
        }
    }
}

void Fishing_FishGiveItem(Actor* this, PlayState* play) {
    ComboItemQuery q;
    ComboItemOverride o;
    int major;

    Fishing_Fish_ItemQuery(play, &q, this->params);

    comboItemOverride(&o, &q);
    major = !isItemFastBuy(o.gi);
    if (major)
    {
        /* "It's smaller than your current fish. Are you sure?" */
        /* Behavior overridden later */
        PlayerDisplayTextBox(play, 0x4098, NULL);
        u8* keepState = (((u8*)this)+0x1C5);
        *keepState = 1; /* Wait for another dialog */
        s16* timer = (s16*)(((u8*)this) + 0x16A);
        *timer = 60;
    }
    else
    {
        Actor_Kill(this);
    }

    comboAddItemEx(play, &q, major);

    /* Play the sound */
    comboPlayItemFanfare(o.gi, 0);

    int flag = this->params - 100;
    if (gSave.age == AGE_ADULT) {
        flag += 17;
    }
    BITMAP8_SET(gSharedCustomSave.caughtFishFlags, flag);
    this->draw = NULL;
}
