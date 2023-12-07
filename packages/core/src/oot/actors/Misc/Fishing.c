#include <combo.h>
#include <combo/item.h>

int Fishing_HasGivenItem(Actor* this)
{
    if (Actor_HasParent(this))
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

void Fishing_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
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

static void Fishing_Fish_ItemQuery(GameState_Play* playOrNull, ComboItemQuery* q, u16 variable)
{
    bzero(q, sizeof(*q));

    q->gi = gSave.age == AGE_ADULT
        ? GI_OOT_FISHING_POND_CHILD_FISH_2LBS
        : GI_OOT_FISHING_POND_ADULT_FISH_4LBS;
    q->ovType = OV_FISH;
    q->id = variable - 100; // EN_FISH_PARAM
    q->giRenew = GI_OOT_RECOVERY_HEART;

    if (gSave.age == AGE_ADULT) {
        q->id += 17;
    }

    if (playOrNull != NULL) {
        int flag = variable - 100;
        if (gSave.age == AGE_ADULT) {
            if (GetCollectibleFlag(playOrNull, flag)) {
                q->ovFlags |= OVF_RENEW;
            }
        } else {
            if (GetChestFlag(playOrNull, flag)) {
                q->ovFlags |= OVF_RENEW;
            }
        }
    }
}

static void Fishing_Fish_ItemOverride(GameState_Play* playOrNull, ComboItemOverride* o, u16 variable)
{
    ComboItemQuery q;

    Fishing_Fish_ItemQuery(playOrNull, &q, variable);
    comboItemOverride(o, &q);
}


void Fishing_DrawFish_SkelAnime(GameState_Play* play, void** skeleton, Vec3s* jointTable, s32 dListCount,
                           void* overrideLimbDraw, void* postLimbDraw, Actor* this) {
    ComboItemOverride o;

    Fishing_Fish_ItemOverride(play, &o, this->variable);
    if (o.gi >= GI_OOT_FISHING_POND_CHILD_FISH_2LBS && o.gi <= GI_OOT_FISHING_POND_ADULT_LOACH_36LBS)
    {
        SkelAnime_DrawFlexOpa(play, skeleton, jointTable, dListCount, overrideLimbDraw, postLimbDraw, this);
        return;
    }

    Vec3f mouthOffset = { 0.0f, 0.0f, 0.0f };
    Vec3f* fishMouthPos = (Vec3f*)(((u8*)this) + 0x1B0);
    Matrix_MultVec3f(&mouthOffset, fishMouthPos);

    ModelViewScale(56.0f, 56.0f, 56.0f, MAT_MUL);
    comboDrawGI(play, this, o.gi, 0);
}

PATCH_CALL(0x80a405d8, Fishing_DrawFish_SkelAnime);
PATCH_CALL(0x80a4066c, Fishing_DrawFish_SkelAnime);

static f32 Fishing_GetFishOnHand(u8* sFishOnHandIsLoach, s32 take) {
    u8 weight = 0;
    if (gSave.age == AGE_ADULT) {
        if (gSharedCustomSave.caughtAdultFishWeight[0]) {
            weight = gSharedCustomSave.caughtAdultFishWeight[gSharedCustomSave.caughtAdultFishWeight[0]];
            if (take) {
                gSharedCustomSave.caughtAdultFishWeight[0]--;
            }
        }
    } else {
        if (gSharedCustomSave.caughtChildFishWeight[0]) {
            weight = gSharedCustomSave.caughtChildFishWeight[gSharedCustomSave.caughtChildFishWeight[0]];
            if (take) {
                gSharedCustomSave.caughtChildFishWeight[0]--;
            }
        }
    }

    if (weight) {
        if (weight & 0x80) {
            // Loach
            *sFishOnHandIsLoach = 1;
        } else {
            // Fish
            *sFishOnHandIsLoach = 0;
        }
        weight &= 0x7F;
        return (f32) weight;
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

    // Displaced code:
    *stateAndTimer = 20;

    return this;
}

void Fishing_SetCaughtId(Actor* actor, u16* sFishingCaughtTextId) {
    // Displaced code:
    u8* stateAndTimer = ((u8*)actor) + 0x1C5;
    *stateAndTimer = 0;

    *sFishingCaughtTextId = actor->variable;
}

void Fishing_HijackCaughtMessage(GameState_Play* play, u16 textId, Actor* this) {
    if (textId >= 200) {
        PlayerDisplayTextBox(play, textId, this);
        return;
    }

    u16 variable = textId;
    textId = 0x4083; // standard fish caught

    PlayerDisplayTextBox(play, textId, this);

    ComboItemOverride o;

    Fishing_Fish_ItemOverride(play, &o, variable);
    comboTextHijackFishCaught(play, &o);
}

PATCH_CALL(0x80a42b98, Fishing_HijackCaughtMessage);

s32 Fishing_IsFishLoach(u16 variable) {
    if (variable == 200) {
        // Fish Tank
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

    return variable < 115;
}

void Fishing_OverrideInitFishLength(u8 linkAge, f32 childMultiplier, Actor* this) {
    f32* fishLength = (f32*)(((u8*)this)+0x19C);

    // Displaced code:
    if (linkAge == AGE_CHILD) {
        *fishLength *= childMultiplier;
    }

    ComboItemOverride o;

    Fishing_Fish_ItemOverride(NULL, &o, this->variable);

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
    }
    else if (o.gi >= GI_OOT_FISHING_POND_ADULT_LOACH_29LBS && o.gi <= GI_OOT_FISHING_POND_ADULT_LOACH_36LBS)
    {
        pounds = o.gi - GI_OOT_FISHING_POND_ADULT_LOACH_29LBS + 29;
    }
    else
    {
        pounds = 0;
    }

    if (pounds)
    {
        *fishLength = FISH_WEIGHT_TO_LENGTH(pounds);
    }
}

void Fishing_FishGiveItem(Actor* this, GameState_Play* play) {
    ComboItemQuery q;
    ComboItemOverride o;
    int major;

    Fishing_Fish_ItemQuery(play, &q, this->variable);

    comboItemOverride(&o, &q);
    major = !isItemFastBuy(o.gi);
    if (major)
    {
        // "It's smaller than your current fish. Are you sure?"
        // Behavior overridden later
        PlayerDisplayTextBox(play, 0x4098, NULL);
        u8* keepState = (((u8*)this)+0x1C5);
        *keepState = 1; // Wait for another dialog
        s16* timer = (s16*)(((u8*)this) + 0x16A);
        *timer = 60;
    }
    else
    {
        ActorDestroy(this);
    }

    /* Play the sound */
    PlaySound(0x4824);

    comboAddItemEx(play, &q, major);

    int flag = this->variable - 100;
    if (gSave.age == AGE_ADULT) {
        SetCollectibleFlag(play, flag);
    } else {
        SetChestFlag(play, flag);
    }

    this->draw = NULL;
}
