#include <combo.h>

#if defined(GAME_OOT)
# define ADDR_INIT              0x80abddbc
# define ADDR_SPAWN_CHILDREN    0x80abdacc
#else
# define ADDR_INIT              0x80961350
# define ADDR_SPAWN_CHILDREN    0x8096104c
#endif

void EnKusa_Aliases(Xflag* xflag);

void ObjMure2_InitWrapper(Actor_ObjMure2* this, GameState_Play* play)
{
    ActorFunc init;

    /* Set the extended properties */
    this->xflag.sceneId = play->sceneId;
    this->xflag.setupId = g.sceneSetupId;
    this->xflag.roomId = this->base.room;
    this->xflag.sliceId = 0;
    this->xflag.id = g.actorIndex;

    /* Forward init */
    init = actorAddr(AC_OBJ_MURE2, ADDR_INIT);
    init(&this->base, play);
}

void ObjMure2_SpawnChildrenWrapper(Actor_ObjMure2* this, GameState_Play* play)
{
    Actor_EnKusa* kusa;
    ActorFunc ObjMure2_SpawnChildren;
    int count;

    /* Call the original function */
    ObjMure2_SpawnChildren = actorAddr(AC_OBJ_MURE2, ADDR_SPAWN_CHILDREN);
    ObjMure2_SpawnChildren(&this->base, play);

    /* Check for type */
    switch (this->base.variable & 3)
    {
    case 0:
        count = 9;
        break;
    case 1:
        count = 12;
        break;
    default:
        return;
    }

    /* Patch the children */
    for (int i = 0; i < count; ++i)
    {
        kusa = (Actor_EnKusa*)this->children[i];
        if (!kusa)
            continue;

        kusa->xflag.sceneId = this->xflag.sceneId;
        kusa->xflag.setupId = this->xflag.setupId;
        kusa->xflag.roomId = this->xflag.roomId;
        kusa->xflag.sliceId = i;
        kusa->xflag.id = this->xflag.id;

        /* Aliases */
        EnKusa_Aliases(&kusa->xflag);
    }
}
