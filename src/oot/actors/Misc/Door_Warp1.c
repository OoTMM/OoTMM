#include <combo.h>

#define ROT_DIVISOR 100

void DoorWarp1_AfterDrawWarp(Actor* this, GameState_Play* play)
{
    float angle;

    if (!(actor->variable & 0xff00))
    {
        angle = (play->gs.frameCount % ROT_DIVISOR) * ((M_PI * 2) / ROT_DIVISOR);

        ModelViewTranslate(this->position.x, this->position.y + 35.f, this->position.z, MAT_SET);
        ModelViewScale(0.35f, 0.35f, 0.35f, MAT_MUL);
        ModelViewRotateY(angle, MAT_MUL);
        comboDrawGI(play, this, GI_OOT_STONE_EMERALD, DRAW_RAW);
    }
}
