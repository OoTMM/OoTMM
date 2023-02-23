#include <combo.h>

typedef struct
{
    Actor base;
}
Actor_CustomWarp;

static void CustomWarp_Init(Actor_CustomWarp* this, GameState_Play* play)
{

}

static void CustomWarp_Update(Actor_CustomWarp* this, GameState_Play* play)
{
    Actor_SetCollisionCylinder(play, &this->base, 20.f, 20.f, 50.f, 4);
}

static void CustomWarp_Draw(Actor_CustomWarp* this, GameState_Play* play)
{
    void* obj;
    u16 objId;
    static const float scale = 0.1f;

    /* Load the warp object */
    objId = 0x143;
#if defined(GAME_OOT)
        objId |= 0x2000;
#endif
    obj = comboGetObject(objId);
    comboSetObjectSegment(play->gs.gfx, obj);

    /* Transform */
    ModelViewTranslate(this->base.position.x, this->base.position.y, this->base.position.z, MAT_SET);
    ModelViewScale(scale, scale, scale, MAT_MUL);

    /* Draw */
    OPEN_DISPS(play->gs.gfx);
    InitListPolyXlu(play->gs.gfx);
    gSPMatrix(POLY_XLU_DISP++, GetMatrixMV(play->gs.gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_XLU_DISP++, 0x06000950);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 0, 255, 255, 255);
    gSPDisplayList(POLY_XLU_DISP++, 0x060009d0);
    CLOSE_DISPS();
}

ActorInit CustomWarp_gActorInit = {
    AC_CUSTOM_WARP,
    0x4,
    0x10,
    0x1,
    sizeof(Actor_CustomWarp),
    (ActorFunc)CustomWarp_Init,
    NULL,
    (ActorFunc)CustomWarp_Update,
    (ActorFunc)CustomWarp_Draw,
};
