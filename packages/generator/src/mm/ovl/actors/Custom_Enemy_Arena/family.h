#ifndef ENEMY_SCRIPTED_FAMILY_H
#define ENEMY_SCRIPTED_FAMILY_H

#include <combo.h>
#include <combo/enemy_scripted.h>

static int scriptedFamily(Actor* root, PlayState* play, s16 id, int hold)
{
    int count = 0;
    for (int category = 0; category < ACTORCAT_MAX; ++category)
        for (Actor* child = play->actorCtx.actors[category].first; child; child = child->next)
            if (child->id == id && child->parent == root && child->room == root->room)
            {
                if (child->init || !child->update)
                    return -1;
                ++count;
                if (hold && !comboEnemyScriptedHoldChild(root, child))
                    return -1;
            }
    return count;
}

#endif
