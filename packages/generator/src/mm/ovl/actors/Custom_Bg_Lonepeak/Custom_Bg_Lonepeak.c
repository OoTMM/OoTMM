#include "Custom_Bg_Lonepeak.h"

#define FLAGS 0

void CustomBgLonepeak_Init(Actor* thisx, PlayState* play);
void CustomBgLonepeak_Destroy(Actor* thisx, PlayState* play);
void CustomBgLonepeak_Update(Actor* thisx, PlayState* play);


/* Raw collision header: */
/*
Min bounds (F6B0, F6C2, DEB4)
Max Bounds (27A7, 085A, 1A92)
Vertices Count (01D7)
Vertices Ptr (020039C0)
Poly Count (02FE)
Poly Ptr (020009E0)
Surfaces (02000918)
*/

static Vec3s sColVertices[] = {
    { 391, 0, -140 },
    { 941, 0, -207 },
    { 379, 174, -99 },
    { 411, 221, 1 },
    { 443, 193, 95 },
    { 462, 0, 126 },
    { 768, 368, -204 },
    { 422, 368, -89 },
    { 465, 368, 38 },
    { 820, 368, -47 },
    { 612, 266, -300 },
    { 384, 266, -228 },
    { 564, 266, 184 },
    { 792, 266, 112 },
    { 390, 271, -75 },
    { 443, 246, 47 },
    { 357, 0, 219 },
    { 503, 0, 151 },
    { 590, 0, 293 },
    { 393, 0, -171 },
    { 277, 161, -183 },
    { 249, 0, -139 },
    { 379, 0, -341 },
    { 594, -242, 229 },
    { 405, -242, 177 },
    { 274, 0, 33 },
    { 286, -211, -133 },
    { 420, -242, -293 },
    { 635, 0, 6 },
    { 589, 0, -164 },
};

static CollisionPoly sColPolys[] = {
    { 0, {{ 0, 1, 2 }}, { 3865, -7210, 31729 }, 1082 },
    { 0, {{ 2, 1, 3 }}, { -5869, -28412, 15232 }, 1072 },
    { 0, {{ 1, 4, 3 }}, { -13854, -29417, -4046 }, 895 },
    { 0, {{ 4, 1, 5 }}, { -18382, -6057, -26441 }, 407 },
    { 0, {{ 6, 7, 8 }}, { 0, 32767, 0 }, -168 },
    { 0, {{ 6, 8, 9 }}, { 0, 32767, 0 }, -168 },
    { 0, {{ 6, 10, 11 }}, { -5851, 26385, -18527 }, -494 },
    { 0, {{ 6, 11, 7 }}, { -5977, 26732, -17982 }, -460 },
    { 0, {{ 8, 12, 13 }}, { 5098, 28055, 16143 }, 97 },
    { 0, {{ 8, 13, 9 }}, { 4328, 26987, 18074 }, 237 },
    { 0, {{ 2, 14, 11 }}, { -32569, 3405, 1166 }, 2465 },
    { 0, {{ 14, 7, 11 }}, { -31068, 10376, 879 }, 2326 },
    { 0, {{ 4, 15, 3 }}, { -27737, 11711, 12931 }, 2594 },
    { 0, {{ 15, 14, 3 }}, { -27915, 9744, 14124 }, 2656 },
    { 0, {{ 14, 2, 3 }}, { -31348, 1214, 9461 }, 2732 },
    { 0, {{ 15, 8, 7 }}, { -30466, 6255, 10315 }, 2707 },
    { 0, {{ 15, 7, 14 }}, { -27389, 11080, 14169 }, 2616 },
    { 0, {{ 16, 12, 17 }}, { -13541, 6712, -29073 }, 58 },
    { 0, {{ 12, 5, 17 }}, { -17057, 441, 27974 }, 2328 },
    { 0, {{ 12, 4, 5 }}, { -20202, 2141, 25710 }, 2497 },
    { 0, {{ 12, 15, 4 }}, { -22570, 15946, 17607 }, 2377 },
    { 0, {{ 12, 8, 15 }}, { -24576, 5969, 20834 }, 2669 },
    { 0, {{ 12, 16, 18 }}, { -9329, 11125, 29375 }, 1739 },
    { 0, {{ 0, 11, 19 }}, { -32662, -1557, -2107 }, 2327 },
    { 0, {{ 0, 2, 11 }}, { -32632, -1672, -2457 }, 2308 },
    { 0, {{ 11, 20, 21 }}, { 5761, 7567, 31356 }, 1039 },
    { 0, {{ 11, 21, 19 }}, { 6947, 6934, 31262 }, 948 },
    { 0, {{ 20, 22, 21 }}, { -27554, -54, -17733 }, 1141 },
    { 0, {{ 22, 11, 10 }}, { -9127, 12450, -28903 }, -713 },
    { 0, {{ 22, 20, 11 }}, { -20164, 10419, -23634 }, 358 },
    { 0, {{ 23, 18, 16 }}, { -9606, -8158, 30246 }, 1671 },
    { 0, {{ 23, 16, 24 }}, { -8489, -7039, 30855 }, 1617 },
    { 0, {{ 24, 25, 26 }}, { -29502, -9815, 10343 }, 2436 },
    { 0, {{ 24, 16, 25 }}, { -29017, -8003, 12948 }, 2515 },
    { 0, {{ 25, 21, 26 }}, { -31971, -5474, 4647 }, 2414 },
    { 0, {{ 26, 21, 22 }}, { -27196, -5267, -17502 }, 1094 },
    { 0, {{ 26, 22, 27 }}, { -25095, -8109, -19446 }, 823 },
    { 2, {{ 25, 5, 0 }}, { 0, 32767, 0 }, 200 },
    { 2, {{ 21, 25, 0 }}, { 0, 32767, 0 }, 200 },
    { 2, {{ 25, 16, 5 }}, { 0, 32767, 0 }, 200 },
    { 2, {{ 16, 17, 5 }}, { 0, 32767, 0 }, 200 },
    { 2, {{ 21, 0, 19 }}, { 0, 32767, 0 }, 200 },
    { 3, {{ 28, 8193, 29 }}, { 0, 32767, 0 }, 200 },
    { 1, {{ 0, 5, 28 }}, { 0, 32767, 0 }, 200 },
    { 1, {{ 0, 28, 29 }}, { 0, 32767, 0 }, 200 },
};

/*
static SurfaceType sColSurfaceTypes[] = {
    {{ 0x00000000, 0x000017CE }},
    {{ 0x00000001, 0x000007C0 }},
    {{ 0x00000002, 0x000017CE }},
    {{ 0x00000400, 0x024C17CE }},
};
*/

static SurfaceType sColSurfaceTypes[] = {
    {{ 0x00000000, 0x000017CE }},
    {{ 0x00000001, 0x000007C0 }},
    {{ 0x00000002, 0x000017CE }},
    {{ 0x00000400, 0x024C17CE }},
};

static BgCamInfo sCamInfo = {};

static CollisionHeader sColHeader = {
    { 249, -242, -341 },
    { 941, 368, 293 },
    ARRAY_COUNT(sColVertices),
    sColVertices,
    ARRAY_COUNT(sColPolys),
    sColPolys,
    sColSurfaceTypes,
    &sCamInfo,
    0,
    NULL,
};

void CustomBgLonepeak_Init(Actor* thisx, PlayState* play) {
    CustomBgLonepeak* this = (CustomBgLonepeak*)thisx;

    thisx->room = (u8)-1;
    thisx->scale.x = 1.f;
    thisx->scale.y = 1.f;
    thisx->scale.z = 1.f;
    DynaPolyActor_Init(&this->dyna, 0);
    DynaPolyActor_LoadMesh(play, &this->dyna, &sColHeader);
}

void CustomBgLonepeak_Destroy(Actor* thisx, PlayState* play) {
    CustomBgLonepeak* this = (CustomBgLonepeak*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void CustomBgLonepeak_Update(Actor* thisx, PlayState* play) {
}

ActorProfile Custom_Bg_Lonepeak_Profile = {
    ACTOR_CUSTOM_BG_LONEPEAK,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(CustomBgLonepeak),
    CustomBgLonepeak_Init,
    CustomBgLonepeak_Destroy,
    CustomBgLonepeak_Update,
    NULL,
};

OVL_INFO_ACTOR(ACTOR_CUSTOM_BG_LONEPEAK, Custom_Bg_Lonepeak_Profile);
