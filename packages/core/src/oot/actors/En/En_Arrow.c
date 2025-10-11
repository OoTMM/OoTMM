#include <combo.h>

static void EnArrow_CheckIcePlatform(Actor* this, PlayState* play)
{
    float height;
    WaterBox* waterBox;
    Vec3f tmp;
    float tmp2;

    if (this->params != 0x04)
        return;

    height = this->world.pos.y;
    if (!WaterBox_GetSurface1(play, &play->colCtx, this->world.pos.x, this->world.pos.z, &height, &waterBox))
        return;
    if (this->world.pos.y >= height)
        return;

    Math_Vec3f_Diff(&this->world.pos, &this->prevPos, &tmp);
    if (tmp.y != 0.0f) {
        tmp2 = sqrtf(SQ(tmp.x) + SQ(tmp.z));
        if (tmp2 != 0.0f) {
            tmp2 = (((height - this->prevPos.y) / tmp.y) * tmp2) / tmp2;
        }
        tmp.x = this->prevPos.x + (tmp.x * tmp2);
        tmp.y = height;
        tmp.z = this->prevPos.z + (tmp.z * tmp2);
    }
    Actor_Spawn(&play->actorCtx, play, ACTOR_BG_ICEFLOE, tmp.x, tmp.y, tmp.z, 0, 0, 0, 300);
    Actor_Kill(this);
}

static void EnArrow_ActionFlyHook(Actor* this)
{
    /* Original call */
    Actor_MoveXZGravity(this);

    if (Config_Flag(CFG_OOT_ICE_ARROW_PLATFORMS))
        EnArrow_CheckIcePlatform(this, gPlay);
}

PATCH_CALL(0x80884e24, EnArrow_ActionFlyHook);
