#include <combo.h>
#include <combo/item.h>
#include <combo/player.h>

static int CustomItem_IsColliding(Actor_CustomItem* this, GameState_Play* play)
{
    Actor_Player* link;
    float dy;

    link = GET_LINK(play);
    if (this->base.xzDistanceFromLink > 30.f)
        return 0;
    dy = link->base.position.y - this->base.position.y;
    if (dy < 0.f)
        dy = -dy;
    if (dy > 20.f)
        return 0;
    return 1;
}

static void CustomItem_ItemQuery(ComboItemQuery* q, Actor_CustomItem* this)
{
    bzero(q, sizeof(*q));
    q->ovType = OV_EXTENDED;
    q->sceneId = this->xflag.sceneId;
    q->roomId = (this->xflag.roomId | ((this->xflag.setupId & 3) << 6));
    q->id = this->xflag.id;
    q->gi = this->extendedGi;
}

static void CustomItem_ItemOverride(ComboItemOverride* o, Actor_CustomItem* this)
{
    ComboItemQuery q;

    CustomItem_ItemQuery(&q, this);
    comboItemOverride(o, &q);
}

static void CustomItem_UpdateDraw(Actor_CustomItem* this)
{
    ComboItemOverride o;

    /* Query the item */
    CustomItem_ItemOverride(&o, this);

    /* Set the draw GI */
    this->extendedGiDraw = o.gi;
}

static void CustomItem_Fini(Actor_CustomItem* this, GameState_Play* play)
{

}

static void CustomItem_HandlerCollected(Actor_CustomItem* this, GameState_Play* play)
{
    Actor_Player* link;

    link = GET_LINK(play);
    this->base.position.y = link->base.position.y + 50.f;
    this->base.position.x = link->base.position.x;
    this->base.position.z = link->base.position.z;

#if defined(GAME_OOT)
    if (gSave.age == AGE_ADULT) this->base.position.y += 20.f;
#endif

    if (this->timer > 0)
        this->timer--;

    if (this->extendedMajor)
    {
        if (Message_IsClosed(&this->base, play))
        {
            UnfreezePlayer(play);
            this->extendedMajor = 0;
        }
        else
        {
            FreezePlayer(play);
            return;
        }
    }

    if (this->timer == 0)
    {
        ActorDestroy(&this->base);
        return;
    }
}

static void CustomItem_Collect(Actor_CustomItem* this, GameState_Play* play)
{
    Actor_Player* link;
    ComboItemQuery q;
    ComboItemOverride o;
    int major;

    link = GET_LINK(play);
    if (link->state & PLAYER_ACTOR_STATE_FROZEN)
        return;

    /* Query the item */
    CustomItem_ItemQuery(&q, this);
    comboItemOverride(&o, &q);
    major = !isItemFastBuy(o.gi);
    if (major)
    {
        PlayerDisplayTextBox(play, 0xb4, NULL);
        FreezePlayer(play);
        this->extendedMajor = 1;
    }
    else
    {
        this->extendedMajor = 0;
    }
    comboAddItemEx(play, &q, major);
    comboXflagsSet(&this->xflag);

    /* Play the sound */
    PlaySound(0x4824);

    /* Disable physics */
    this->base.gravity = 0.f;
    this->base.velocity.y = 0.f;

    /* Set the timer */
    this->timer = 20;

    /* Set the collected handler */
    this->handler = CustomItem_HandlerCollected;
    this->handler(this, play);
}

static void CustomItem_HandlerUncollected(Actor_CustomItem* this, GameState_Play* play)
{
    /* Enable physics */
    ActorUpdateVelocity(&this->base);
    Actor_SetCollisionCylinder(play, &this->base, 10.f, 15.f, 15.f, 0x1d);
    CustomItem_UpdateDraw(this);

    /* Check for a collision */
    if (CustomItem_IsColliding(this, play))
        CustomItem_Collect(this, play);
}

static void CustomItem_Update(Actor_CustomItem* this, GameState_Play* play)
{
    /* Invoke the handler */
    this->handler(this, play);
}

static void CustomItem_Draw(Actor_CustomItem* this, GameState_Play* play)
{
    static const float scale = 12.5f;
    static const int kRotDivisor = 60;
    float angle;

    angle = (play->gs.frameCount % kRotDivisor) * (1.f / kRotDivisor) * M_PI * 2.f;

    ModelViewScale(scale, scale, scale, MAT_MUL);
    ModelViewTranslate(0, 35.f, 0, MAT_MUL);
    ModelViewRotateY(angle, MAT_MUL);
    comboDrawGI(play, &this->base, this->extendedGiDraw, 0);
}

static void CustomItem_Init(Actor_CustomItem* this, GameState_Play* play)
{
#if defined(GAME_OOT)
    this->extendedGi = GI_OOT_RUPEE_GREEN;
#else
    this->extendedGi = GI_MM_RUPEE_GREEN;
#endif

    ActorSetScale(&this->base, 0.015f);
    this->handler = CustomItem_HandlerUncollected;
}

#if defined(GAME_OOT)
ActorInit CustomItem_gActorInit = {
    AC_CUSTOM_ITEM,
    0x8,
    0x10,
    0x1,
    sizeof(Actor_CustomItem),
    (ActorFunc)CustomItem_Init,
    (ActorFunc)CustomItem_Fini,
    (ActorFunc)CustomItem_Update,
    (ActorFunc)CustomItem_Draw,
};

Actor_CustomItem* DropCustomItem(GameState_Play* play, const Vec3f* pos, const Xflag* xflag)
{
    Actor_CustomItem* item;

    /* Spawn the custom actor */
    item = (Actor_CustomItem*)SpawnActor(
#if defined(GAME_OOT)
        (char*)play + 0x1c24,
#else
        (char*)play + 0x1ca0,
#endif
        play,
        AC_CUSTOM_ITEM,
        pos->x, pos->y, pos->z,
        0, 0, 0,
        0
    );

    if (!item)
        return NULL;

    /* Copy the extended flag */
    memcpy(&item->xflag, xflag, sizeof(Xflag));

    /* Set the draw GI */
    CustomItem_UpdateDraw(item);

    /* Set the physics */
    item->base.gravity = -0.9f;
    item->base.velocity.y = 8.0f;
    item->base.speedXZ = 2.0f;
    item->base.speedRot.y = (s16)((RandFloat() - 0.5f) * 65536.f);

    return item;
}
#endif
