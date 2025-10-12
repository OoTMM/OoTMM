#include "Obj_Yasi.h"
#include <combo/draw.h>
#include <assets/mm/objects/object_obj_yasi.h>

#define FLAGS 0x00000000

#define CAN_DROP_NUT(thisx) (thisx->params < 0)

void ObjYasi_Init(Actor* thisx, PlayState* play);
void ObjYasi_Destroy(Actor* thisx, PlayState* play);
void ObjYasi_Update(Actor* thisx, PlayState* play);
void ObjYasi_Draw(Actor* thisx, PlayState* play);

ActorProfile Obj_Yasi_Profile = {
    /**/ ACTOR_OBJ_YASI,
    /**/ ACTORCAT_PROP,
    /**/ FLAGS,
    /**/ OBJECT_OBJ_YASI,
    /**/ sizeof(ObjYasi),
    /**/ ObjYasi_Init,
    /**/ ObjYasi_Destroy,
    /**/ ObjYasi_Update,
    /**/ ObjYasi_Draw,
};

OVL_INFO_ACTOR(ACTOR_OBJ_YASI, Obj_Yasi_Profile);

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDistance, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeScale, 400, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDownward, 800, ICHAIN_STOP),
};

static void ObjYasi_Alias(Xflag* xf)
{
    switch (xf->sceneId)
    {
    case SCE_MM_ZORA_CAPE:
        if (xf->setupId == 1)
        {
            xf->setupId = 0;
            xf->id -= 1;
        }
        break;
    case SCE_MM_GREAT_BAY_COAST:
        if (xf->setupId == 1)
        {
            xf->setupId = 0;
            switch (xf->id)
            {
            case 31: xf->id = 10; break;
            case 32: xf->id = 13; break;
            case 33: xf->id = 11; break;
            case 34: xf->id = 12; break;
            }
        }
        break;
    }
}

void ObjYasi_Init(Actor* thisx, PlayState* play) {
    ObjYasi* this = (ObjYasi*)thisx;

    if (comboXflagInit(&this->xflag, thisx, play)) {
        ObjYasi_Alias(&this->xflag);
    }

    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    DynaPolyActor_Init(&this->dyna, 0);
    DynaPolyActor_LoadMesh(play, &this->dyna, (void*)&gPalmTreeCol);

    this->dyna.actor.home.rot.y = 0;

    if (PALM_TREE_IS_WIDE(thisx)) {
        this->dyna.actor.scale.x = 0.2f;
        this->dyna.actor.scale.z = 0.2f;
    }
}

void ObjYasi_Destroy(Actor* thisx, PlayState* play) {
    ObjYasi* this = (ObjYasi*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

static int ObjYasi_DropCustom(ObjYasi* this, PlayState* play)
{
    if (Xflag_IsShuffled(&this->xflag))
    {
        EnItem00_DropCustom(play, &this->dyna.actor.world.pos, &this->xflag);
        return true;
    }

    return false;
}

void ObjYasi_Update(Actor* thisx, PlayState* play) {
    ObjYasi* this = (ObjYasi*)thisx;
    s16 temp;
    Vec3f dropPos;

    if (this->dyna.actor.home.rot.z != 0) {
        if (!ObjYasi_DropCustom(this, play) && CAN_DROP_NUT(thisx)) {
            if (Rand_ZeroOne() < 0.5f) {
                dropPos.x = this->dyna.actor.world.pos.x;
                dropPos.y = this->dyna.actor.world.pos.y + 280.0f;
                dropPos.z = this->dyna.actor.world.pos.z;
                Item_DropCollectible(play, &dropPos, ITEM00_NUT);
            }
        }
        this->dyna.actor.home.rot.y = GET_PLAYER(play)->actor.shape.rot.y;
        this->dyna.actor.home.rot.x = 400;
        this->dyna.actor.home.rot.z = 0;
    }
    temp = TRUNCF_BINANG(this->dyna.actor.shape.rot.x * 0.1f);
    this->dyna.actor.home.rot.x -= temp;
    this->dyna.actor.shape.rot.x +=
        BINANG_SUB(this->dyna.actor.home.rot.x, TRUNCF_BINANG(this->dyna.actor.shape.rot.x * 0.08f));
}

static Gfx sDisplayListNuts[] = {
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(0x06000e50, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, 4, 4, 0, 0),
    gsSPLoadGeometryMode(G_TEXTURE_ENABLE | G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(0x060000F0, 9, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP1Triangle(6, 7, 8, 0),
    gsSPEndDisplayList(),
};

void ObjYasi_Draw(Actor* thisx, PlayState* play) {
    ObjYasi* this = (ObjYasi*)thisx;

    Matrix_Translate(this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y, this->dyna.actor.world.pos.z,
                     MTXMODE_NEW);

    if (this->dyna.actor.shape.rot.x != 0) {
        Matrix_RotateYS(this->dyna.actor.home.rot.y, MTXMODE_APPLY);
        Matrix_RotateXS(this->dyna.actor.shape.rot.x, MTXMODE_APPLY);
        Matrix_RotateYS(BINANG_SUB(this->dyna.actor.shape.rot.y, this->dyna.actor.home.rot.y), MTXMODE_APPLY);
    } else {
        Matrix_RotateYS(this->dyna.actor.shape.rot.y, MTXMODE_APPLY);
    }

    Matrix_Scale(0.1f, 0.1f, 0.1f, MTXMODE_APPLY);
    Gfx_DrawDListOpa(play, gPalmTreeDL);

    OPEN_DISPS(play->state.gfxCtx);
    if (!Xflag_IsShuffled(&this->xflag))
    {
        gSPDisplayList(POLY_OPA_DISP++, sDisplayListNuts);
    }
    else
    {
        ComboItemOverride o;

        comboXflagItemOverride(&o, &this->xflag, 0);
        Matrix_Translate(this->dyna.actor.world.pos.x + 5.f, this->dyna.actor.world.pos.y + 290.f, this->dyna.actor.world.pos.z, MTXMODE_NEW);
        Matrix_Scale(0.5f, 0.5f, 0.5f, MTXMODE_APPLY);
        Matrix_RotateY(this->dyna.actor.shape.rot.y * (M_PI / 0x8000), MTXMODE_APPLY);
        Draw_Gi(play, &this->dyna.actor, o.cloakGi ? o.cloakGi : o.gi, 0);
    }
    CLOSE_DISPS();
}
