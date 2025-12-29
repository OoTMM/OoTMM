#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/config.h>
#include <combo/draw.h>

static void EnItem00_ItemQuery(ComboItemQuery* q, Actor_EnItem00* this, PlayState* play, s16 gi)
{
    memset(q, 0, sizeof(*q));

    if (this->actor.params == 0x06)
    {
        q->ovType = OV_COLLECTIBLE;
        q->sceneId = comboSceneKey(play->sceneId);
        q->id = this->collectibleFlag;
        q->gi = gi;
    }
    else
    {
        q->ovType = OV_NONE;
        q->gi = gi;
    }
}

void EnItem00_GiveItem(Actor_EnItem00* this, PlayState* play, s16 gi, float a, float b)
{
    ComboItemQuery q;
    Player* link;
    s16 itemId;

    EnItem00_ItemQuery(&q, this, play, gi);
    link = GET_PLAYER(play);
    itemId = -1;

    if (q.ovType == OV_NONE)
    {
        itemId = kExtendedGetItems[gi - 1].itemId;
        if (GetItemCollectBehavior(itemId) == 0xff)
            itemId = -1;

        if (gi == GI_MM_SHIELD_HERO && gSharedCustomSave.mmShieldIsDeku)
            q.gi = GI_MM_SHIELD_DEKU;
    }

    if (itemId >= 0)
    {
        this->actor.parent = &link->actor;
        AddItemWithIcon(play, link, &kExtendedGetItems[gi - 1]);
        return;
    }

    comboGiveItem(&this->actor, play, &q, a, b);
}

PATCH_CALL(0x800a700c, EnItem00_GiveItem);
PATCH_CALL(0x800a6a8c, EnItem00_GiveItem);

void EnItem00_DrawHeartPiece(Actor_EnItem00* this, PlayState* play)
{
    static const float scale = 17.5f;

    ComboItemQuery q;
    ComboItemOverride o;
    s16 gi;

    EnItem00_ItemQuery(&q, this, play, GI_OOT_HEART_PIECE);
    comboItemOverride(&o, &q);
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gi = o.gi;
    if (o.cloakGi)
    {
        gi = o.cloakGi;
        Matrix_RotateYS(-this->actor.shape.rot.y * 2, MTXMODE_APPLY);
    }
    Draw_Gi(play, &this->actor, gi, 0);
}

static int dropBombs(int dropId)
{
    int hasChuBag;
    int hasBombBag;
    int hasBombBagOot;
    u8  bombCount;
    u8  bombCountOot;
    u8  bombchuCount;

    hasChuBag = (gSharedCustomSave.bombchuBagMm > 0);
    hasBombBag = (gSave.info.inventory.upgrades.bombBag > 0);
    hasBombBagOot = (gMmExtraItems.bombSlot & 2);

    if (!hasChuBag && !hasBombBagOot)
    {
        if (!hasBombBag)
            return -1;
        return dropId;
    }

    if (!hasBombBag && !hasBombBagOot)
        return ITEM00_BOMBCHU;

    if (!hasBombBag && !hasChuBag)
        return ITEM00_BOMBS_OOT_5;

    /* We have at least two, check for ammo */
    bombCount = hasBombBag ? gSave.info.inventory.ammo[ITS_MM_BOMBS] : 15;
    bombchuCount = hasChuBag ? gSave.info.inventory.ammo[ITS_MM_BOMBCHU] : 15;
    bombCountOot = hasBombBagOot ? gMmExtraAmmo.ootBombAmmo : 15;

    /* Low on ammo */
    if (bombCount < 15 || bombchuCount < 15 || bombCountOot < 15)
    {
        if (bombchuCount < bombCount && bombchuCount < bombCountOot)
            return ITEM00_BOMBCHU;
        if (bombCountOot < bombCount && bombCountOot < bombchuCount)
            return ITEM00_BOMBS_OOT_5;
        return dropId;
    }

    if (!hasBombBag)
    {
        if (Rand_ZeroOne() < 0.5f)
            return ITEM00_BOMBCHU;
        return ITEM00_BOMBS_OOT_5;
    }

    if (!hasChuBag)
    {
        if (Rand_ZeroOne() < 0.5f)
            return ITEM00_BOMBS_OOT_5;
        return dropId;
    }

    if (!hasBombBagOot)
    {
        if (Rand_ZeroOne() < 0.5f)
            return ITEM00_BOMBCHU;
        return dropId;
    }

    f32 rand = Rand_ZeroFloat(3.0f);

    if (rand < 1.0f)
        return ITEM00_BOMBCHU;
    if (rand < 2.0f)
        return ITEM00_BOMBS_OOT_5;
    return dropId;
}

/* TODO: Flexible drops would ideally need to be patched on top of this */
int EnItem00_FixDrop(int dropId)
{
    switch (dropId)
    {
    case ITEM00_ARROWS_10:
    case ITEM00_ARROWS_30:
    case ITEM00_ARROWS_40:
    case ITEM00_ARROWS_50:
        if (gSave.info.inventory.upgrades.quiver == 0)
            dropId = -1;
        break;
    case ITEM00_BOMB:
    case ITEM00_BOMBS_5:
        dropId = dropBombs(dropId);
        break;
    case ITEM00_RECOVERY_HEART:
        if (gSave.info.playerData.health >= gSave.info.playerData.healthCapacity)
            dropId = ITEM00_RUPEE_GREEN;
        break;
    case ITEM00_MAGIC_SMALL:
    case ITEM00_MAGIC_LARGE:
        if (!gSave.info.playerData.isMagicAcquired)
            dropId = -1;
        break;
    default:
        break;
    }

    return dropId;
}

PATCH_CALL(0x800a7994, EnItem00_FixDrop);
PATCH_CALL(0x800a7c44, EnItem00_FixDrop);
PATCH_CALL(0x800a8024, EnItem00_FixDrop);

void EnItem00_AliasFreestandingRupee(Xflag* xflag)
{
    switch (xflag->sceneId)
    {
    case SCE_MM_WOODFALL:
        if (xflag->setupId == 2)
        {
            xflag->setupId = 0;
            xflag->id -= 37;
        }
        break;
    case SCE_MM_LAUNDRY_POOL:
        switch (xflag->id)
        {
        case 0x14:
        case 0x15:
            xflag->id = 0x13;
            break;
        case 0x17:
        case 0x18:
            xflag->id = 0x16;
            break;
        case 0x1a:
        case 0x1b:
            xflag->id = 0x19;
            break;
        }
        break;
    case SCE_MM_TWIN_ISLANDS_WINTER:
        xflag->sceneId = SCE_MM_TWIN_ISLANDS_SPRING;
        xflag->id -= 18;
        break;
    case SCE_MM_TEMPLE_STONE_TOWER_INVERTED:
        if (xflag->roomId == 0x03)
        {
            switch (xflag->id)
            {
            case 0: xflag->roomId = 0x01; xflag->id = 0; break;
            case 3: xflag->roomId = 0x01; xflag->id = 1; break;
            case 4: xflag->roomId = 0x01; xflag->id = 2; break;
            }
        }
        break;
    }
}

void EnItem00_AliasFreestandingHeart(Xflag* xflag)
{
}

void EnItem00_DrawShield(PlayState* play)
{
    s16 gi;

    if (gSharedCustomSave.mmShieldIsDeku)
        gi = GI_MM_SHIELD_DEKU;
    else
        gi = GI_MM_SHIELD_HERO;


    Draw_Gi(play, NULL, gi, DRAW_RAW);
}

void EnItem00_InitType(Actor_EnItem00* this, PlayState* play, f32* shadowOffset, f32* shadowScale, s32* sp30)
{
    Actor* thisx = &this->actor;
    this->blinkMask = 1;
    switch (thisx->params)
    {
    case ITEM00_RUPEE_GREEN:
    case ITEM00_RUPEE_BLUE:
    case ITEM00_RUPEE_RED:
        Actor_SetScale(thisx, 0.015f);
        this->scale = 0.015f;
        *shadowOffset = 750.0f;
        break;

    case ITEM00_KEY:
        this->blinkMask = 0;
        Actor_SetScale(thisx, 0.03f);
        this->scale = 0.03f;
        *shadowOffset = 350.0f;
        break;

    case ITEM00_HEART_PIECE:
    case ITEM00_HEART_CONTAINER:
        this->blinkMask = 0;
        Actor_SetScale(thisx, 0.02f);
        this->scale = 0.02f;
        *shadowOffset = 650.0f;
        if (thisx->params == ITEM00_HEART_CONTAINER)
        {
            *sp30 = -1;
        }
        break;

    case ITEM00_RECOVERY_HEART:
        thisx->home.rot.z = Rand_CenteredFloat(0xFFFF);
        *shadowOffset = 430.0f;
        Actor_SetScale(thisx, 0.02f);
        this->scale = 0.02f;
        break;

    case ITEM00_ARROWS_10:
    case ITEM00_ARROWS_30:
    case ITEM00_ARROWS_40:
    case ITEM00_ARROWS_50:
        Actor_SetScale(thisx, 0.035f);
        this->scale = 0.035f;
        *shadowOffset = 250.0f;
        break;

    case ITEM00_BOMB:
    case ITEM00_BOMBS_5:
    case ITEM00_NUT:
    case ITEM00_STICK:
    case ITEM00_MAGIC_SMALL:
    case ITEM00_NUTS_10:
    case ITEM00_BOMBCHU:
    case ITEM00_BOMBS_OOT_5:
        Actor_SetScale(thisx, 0.03f);
        this->scale = 0.03f;
        *shadowOffset = 320.0f;
        break;

    case ITEM00_MAGIC_LARGE:
        Actor_SetScale(thisx, 4.5f * 0.01f);
        this->scale = 4.5f * 0.01f;
        *shadowOffset = 320.0f;
        break;

    case ITEM00_RUPEE_ORANGE:
        Actor_SetScale(thisx, 4.5f * 0.01f);
        this->scale = 4.5f * 0.01f;
        *shadowOffset = 750.0f;
        break;

    case ITEM00_RUPEE_PURPLE:
        Actor_SetScale(thisx, 0.03f);
        this->scale = 0.03f;
        *shadowOffset = 750.0f;
        break;

    case ITEM00_FLEXIBLE:
    case ITEM00_BIG_FAIRY:
        *shadowOffset = 500.0f;
        Actor_SetScale(thisx, 0.01f);
        this->scale = 0.01f;
        break;

    case ITEM00_SHIELD_HERO:
        thisx->objectSlot = Object_GetSlot(&play->objectCtx, OBJECT_GI_SHIELD_2);
        EnItem00_SetObject(this, play, shadowOffset, shadowScale);
        break;

    case ITEM00_MAP:
        thisx->objectSlot = Object_GetSlot(&play->objectCtx, OBJECT_GI_MAP);
        EnItem00_SetObject(this, play, shadowOffset, shadowScale);
        break;

    case ITEM00_COMPASS:
        thisx->objectSlot = Object_GetSlot(&play->objectCtx, OBJECT_GI_COMPASS);
        EnItem00_SetObject(this, play, shadowOffset, shadowScale);
        break;

    default:
        break;
    }
}

void EnItem00_InitGiveItem(Actor_EnItem00* this, PlayState* play, s32* getItemId)
{
    Actor* thisx = &this->actor;
    switch (thisx->params)
    {
    case ITEM00_RUPEE_GREEN:
        AddItem(play, ITEM_RUPEE_GREEN);
        break;

    case ITEM00_RUPEE_BLUE:
        AddItem(play, ITEM_RUPEE_BLUE);
        break;

    case ITEM00_RUPEE_RED:
        AddItem(play, ITEM_RUPEE_RED);
        break;

    case ITEM00_RUPEE_PURPLE:
        AddItem(play, ITEM_RUPEE_PURPLE);
        break;

    case ITEM00_RUPEE_ORANGE:
        AddItem(play, ITEM_RUPEE_GOLD);
        break;

    case ITEM00_RECOVERY_HEART:
        AddItem(play, ITEM_RECOVERY_HEART);
        break;

    case ITEM00_FLEXIBLE:
    case ITEM00_BIG_FAIRY:
        Health_ChangeBy(play, 0x70);
        break;

    case ITEM00_BOMB:
    case ITEM00_BOMBS_5:
        AddItem(play, ITEM_MM_BOMBS_5);
        break;

    case ITEM00_ARROWS_10:
        AddItem(play, ITEM_MM_ARROWS_10);
        break;

    case ITEM00_ARROWS_30:
        AddItem(play, ITEM_MM_ARROWS_30);
        break;

    case ITEM00_ARROWS_40:
        AddItem(play, ITEM_MM_ARROWS_40);
        break;

    case ITEM00_ARROWS_50:
        AddItem(play, ITEM_MM_ARROWS_50);
        break;

    case ITEM00_MAGIC_LARGE:
        AddItem(play, ITEM_MM_MAGIC_JAR_LARGE);
        break;

    case ITEM00_MAGIC_SMALL:
        AddItem(play, ITEM_MM_MAGIC_JAR_SMALL);
        break;

    case ITEM00_KEY:
        AddItem(play, ITEM_MM_SMALL_KEY);
        break;

    case ITEM00_NUT:
        *getItemId = GI_MM_NUT;
        break;

    case ITEM00_NUTS_10:
        *getItemId = GI_MM_NUTS_10;
        break;

    case ITEM00_BOMBCHU:
        AddItem(play, ITEM_MM_BOMBCHU_5);
        break;

    default:
        break;
    }
}

void EnItem00_UpdateGiveItem(Actor_EnItem00* this, PlayState* play, s32* getItemId)
{
    switch (this->actor.params)
    {
    case ITEM00_RUPEE_GREEN:
        this->rupeeCollected = 1;
        EnItem00_AddXflag(this);
        break;

    case ITEM00_RUPEE_BLUE:
        this->rupeeCollected = 1;
        AddItem(play, ITEM_RUPEE_BLUE);
        break;

    case ITEM00_RUPEE_RED:
        this->rupeeCollected = 1;
        AddItem(play, ITEM_RUPEE_RED);
        break;

    case ITEM00_RUPEE_PURPLE:
        this->rupeeCollected = 1;
        AddItem(play, ITEM_RUPEE_PURPLE);
        break;

    case ITEM00_RUPEE_ORANGE:
        this->rupeeCollected = 1;
        AddItem(play, ITEM_RUPEE_GOLD);
        break;

    case ITEM00_STICK:
        *getItemId = GI_MM_STICK;
        break;

    case ITEM00_NUT:
        *getItemId = GI_MM_NUT;
        break;

    case ITEM00_NUTS_10:
        *getItemId = GI_MM_NUTS_10;
        break;

    case ITEM00_RECOVERY_HEART:
        AddItem(play, ITEM_RECOVERY_HEART);
        break;

    case ITEM00_FLEXIBLE:
    case ITEM00_BIG_FAIRY:
        Health_ChangeBy(play, 0x70);
        break;

    case ITEM00_BOMB:
    case ITEM00_BOMBS_5:
        AddItem(play, ITEM_MM_BOMBS_5);
        break;

    case ITEM00_ARROWS_10:
        AddItem(play, ITEM_MM_ARROWS_10);
        break;

    case ITEM00_ARROWS_30:
        AddItem(play, ITEM_MM_ARROWS_30);
        break;

    case ITEM00_ARROWS_40:
        AddItem(play, ITEM_MM_ARROWS_40);
        break;

    case ITEM00_ARROWS_50:
        AddItem(play, ITEM_MM_ARROWS_50);
        break;

    case ITEM00_KEY:
        *getItemId = GI_MM_SMALL_KEY;
        break;

    case ITEM00_HEART_PIECE:
        *getItemId = GI_MM_HEART_PIECE;
        break;

    case ITEM00_HEART_CONTAINER:
        *getItemId = GI_MM_HEART_CONTAINER;
        break;

    case ITEM00_MAGIC_LARGE:
        AddItem(play, ITEM_MM_MAGIC_JAR_LARGE);
        break;

    case ITEM00_MAGIC_SMALL:
        AddItem(play, ITEM_MM_MAGIC_JAR_SMALL);
        break;

    case ITEM00_SHIELD_HERO:
        *getItemId = GI_MM_SHIELD_HERO;
        break;

    case ITEM00_MAP:
        *getItemId = GI_MM_MAP;
        break;

    case ITEM00_COMPASS:
        *getItemId = GI_MM_COMPASS;
        break;

    case ITEM00_BOMBCHU:
        AddItem(play, ITEM_MM_BOMBCHU_5);
        break;

    case ITEM00_BOMBS_OOT_5:
        comboAddItemRaw(play, GI_MM_BOMBS_OOT_5);
        break;

    default:
        break;
    }
}

extern u8 gDropArrows2Tex[];
extern u8 gDropArrows1Tex[];
extern u8 gDropArrows3Tex[];
extern u8 gDropBombTex[];
extern u8 gDropBombchuTex[];
extern u8 gDropUnknownPouchTex[];
extern u8 gDropRecoveryHeartTex[];
extern u8 gDropHeartPieceTex[];
extern Gfx gItemDropDL[];
extern u8 gDropKeySmallTex[];
extern u8 gDropMagicSmallTex[];
extern u8 gDropDekuNutTex[];
extern u8 gDropDekuStickTex[];
extern u8 gDropMagicLargeTex[];

TexturePtr sItemDropTextures[] = {
    gDropRecoveryHeartTex,
    gDropBombTex,
    gDropArrows1Tex,
    gDropArrows2Tex,
    gDropArrows3Tex,
    gDropBombTex,
    gDropDekuNutTex,
    gDropDekuStickTex,
    gDropMagicLargeTex,
    gDropMagicSmallTex,
    gDropBombchuTex,
    gDropKeySmallTex,
};

void EnItem00_DrawSprite(Actor_EnItem00* this, PlayState* play)
{
    s32 texIndex = this->actor.params - 3;

    OPEN_DISPS(play->state.gfxCtx);

    POLY_OPA_DISP = Play_SetFog(play, POLY_OPA_DISP);

    if (this->actor.params == ITEM00_BOMBS_OOT_5)
    {
        texIndex = 1;
    }
    else if (this->actor.params == ITEM00_NUTS_10)
    {
        texIndex = 6;
    }
    else if (this->actor.params == ITEM00_BOMBCHU)
    {
        texIndex = 10;
    }
    else if (this->actor.params >= ITEM00_ARROWS_30)
    {
        texIndex -= 3;
        if (this->actor.params < ITEM00_ARROWS_50)
        {
            texIndex++;
        }
    }

    POLY_OPA_DISP = Gfx_SetupDL66(POLY_OPA_DISP);

    gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(sItemDropTextures[texIndex]));

    gSPMatrix(POLY_OPA_DISP++, Matrix_Finalize(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD);

    gSPDisplayList(POLY_OPA_DISP++, gItemDropDL);

    CLOSE_DISPS();
}

void EnItem00_Draw(Actor_EnItem00* this, PlayState* play)
{
    if (!(this->blinkTimer & this->blinkMask))
    {
        switch (this->actor.params) {
        case ITEM00_RUPEE_GREEN:
        case ITEM00_RUPEE_BLUE:
        case ITEM00_RUPEE_RED:
        case ITEM00_RUPEE_ORANGE:
        case ITEM00_RUPEE_PURPLE:
            EnItem00_DrawRupee(this, play);
            break;

        case ITEM00_HEART_PIECE:
            EnItem00_DrawHeartPiece(this, play);
            break;

        case ITEM00_HEART_CONTAINER:
            EnItem00_DrawHeartContainer(this, play);
            break;

        case ITEM00_RECOVERY_HEART:
            if (this->timer < 0)
            {
                if (this->timer == -1)
                {
                    s8 objectSlot = Object_GetSlot(&play->objectCtx, OBJECT_GI_HEART);

                    if (Object_IsLoaded(&play->objectCtx, objectSlot))
                    {
                        this->actor.objectSlot = objectSlot;
                        Actor_SetObjectDependency(play, &this->actor);
                        this->timer = -2;
                    }
                }
                else
                {
                    Matrix_Scale(16.0f, 16.0f, 16.0f, MTXMODE_APPLY);
                    DrawGi_Table(play, DRAWGI_RECOVERY_HEART - 1);
                }
                break;
            }
            // fallthrough
        case ITEM00_BOMB:
        case ITEM00_ARROWS_10:
        case ITEM00_ARROWS_30:
        case ITEM00_ARROWS_40:
        case ITEM00_ARROWS_50:
        case ITEM00_BOMBS_5:
        case ITEM00_NUT:
        case ITEM00_STICK:
        case ITEM00_MAGIC_LARGE:
        case ITEM00_MAGIC_SMALL:
        case ITEM00_KEY:
        case ITEM00_NUTS_10:
        case ITEM00_BOMBCHU:
        case ITEM00_BOMBS_OOT_5:
            EnItem00_DrawSprite(this, play);
            break;

        case ITEM00_SHIELD_HERO:
            EnItem00_DrawShield(play);
            break;

        case ITEM00_MAP:
            DrawGi_Table(play, DRAWGI_MAP - 1);
            break;

        case ITEM00_COMPASS:
            DrawGi_Table(play, DRAWGI_COMPASS - 1);
            break;

        case ITEM00_MASK:
        case ITEM00_FLEXIBLE:
        case ITEM00_3_HEARTS:
        case ITEM00_NOTHING:
        case ITEM00_BIG_FAIRY:
            break;
        }
    }
}

PATCH_FUNC(0x800a7128, EnItem00_Draw)

void EnItem00_SetSpawningRotation(Actor_EnItem00* this)
{
    switch (this->actor.params)
    {
    case ITEM00_RUPEE_GREEN:
    case ITEM00_RUPEE_BLUE:
    case ITEM00_RUPEE_RED:
        this->actor.shape.rot.y += 0x3C0;
        break;
    case ITEM00_SHIELD_HERO:
    case ITEM00_NOTHING:
    case ITEM00_BIG_FAIRY:
    case ITEM00_MAP:
    case ITEM00_COMPASS:
    case ITEM00_MUSHROOM_CLOUD:
        this->actor.world.rot.x -= 0x2BC;
        this->actor.shape.rot.y += 0x190;
        this->actor.shape.rot.x = this->actor.world.rot.x - 0x4000;
        break;
    }
}
