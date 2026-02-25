#include <combo.h>
#include <combo/oot/actors/En_Part.h>

void func_80ACE13C(EnPart *this, PlayState *play)
{
    Player* player = GET_PLAYER(play);
    s32 i;
    Vec3f pos;
    Vec3f velocity = {0.0f, 0.0f, 0.0f};
    Vec3f accel = {0.0f, 0.0f, 0.0f};
    Vec3f zeroVec = {0.0f, 0.0f, 0.0f};

    if ((this->actor.params == 12) || (this->actor.params == 13))
    {
        Actor_UpdateBgCheckInfo(play, &this->actor, 5.0f, 15.0f, 0.0f,
                                UPDBGCHECKINFO_FLAG_OOT_2 |
                                UPDBGCHECKINFO_FLAG_OOT_3 |
                                UPDBGCHECKINFO_FLAG_OOT_4
                            );

        if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) ||
            (this->actor.world.pos.y <= this->actor.floorHeight) ||
            (this->actor.world.pos.y <= player->actor.floorHeight)
        )
        {
            this->action = 4;
            this->actor.speed = 0.0f;
            this->actor.gravity = 0.0f;
            this->actor.velocity.y = 0.0f;
        }

        if ((this->actor.params == 13) && (this->actor.parent != NULL) && (this->actor.parent->update == NULL))
        {
            this->actor.parent = NULL;
        }
    }
    else if (this->timer <= 0)
    {
        switch (this->actor.params)
        {
        case 1:
        case 9:
        case 10:
        case 14:
            EffectSsDeadDb_Spawn(play, &this->actor.world.pos, &zeroVec, &zeroVec,
                                 (s16)(this->actor.scale.y * 100.0f) * 40, 7, 255, 255, 255, 255, 0, 255, 0, 1, 9,
                                 true);
            break;
        case 3:
        case 11:
            EffectSsDeadDb_Spawn(play, &this->actor.world.pos, &zeroVec, &zeroVec,
                                 (s16)(this->actor.scale.y * 100.0f) * 40, 7, 255, 255, 255, 255, 0, 0, 255, 1, 9,
                                 true);
            break;
        case 4:
            for (i = 7; i >= 0; i--)
            {
                pos.x = this->actor.world.pos.x + Rand_CenteredFloat(60.0f);
                pos.y = this->actor.world.pos.y + this->actor.shape.yOffset * this->actor.scale.y +
                        Rand_CenteredFloat(50.0f);
                pos.z = this->actor.world.pos.z + Rand_CenteredFloat(60.0f);
                velocity.y = Rand_ZeroOne() + 1.0f;
                EffectSsDtBubble_SpawnColorProfile(play, &pos, &velocity, &accel, Rand_S16Offset(80, 100), 25, 0,
                                                   true);
            }
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            for (i = 4; i >= 0; i--)
            {
                pos.x = this->actor.world.pos.x + Rand_CenteredFloat(25.0f);
                pos.y = this->actor.world.pos.y + Rand_CenteredFloat(40.0f);
                pos.z = this->actor.world.pos.z + Rand_CenteredFloat(25.0f);
                EffectSsDeadDb_Spawn(play, &pos, &zeroVec, &zeroVec, 40, 7, 255, 255, 255, 255, 0, 0, 255, 1, 9,
                                     true);
            }
            break;
        }

        Actor_Kill(&this->actor);
        return;
    }

    this->timer--;
    this->rotZ += this->rotZSpeed;
}

PATCH_FUNC(0x80865894, func_80ACE13C);
