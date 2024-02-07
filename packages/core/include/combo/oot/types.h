#ifndef INCLUDED_OOT_TYPES_H
#define INCLUDED_OOT_TYPES_H

#include <combo/util.h>

typedef struct {
    /* 0x000 */ s32 magic; // string literal "VIEW" / 0x56494557
    /* 0x004 */ struct GraphicsContext* gfxCtx;
    /* 0x008 */ Viewport viewport;
    /* 0x018 */ f32 fovy;  // vertical field of view in degrees
    /* 0x01C */ f32 zNear; // distance to near clipping plane
    /* 0x020 */ f32 zFar;  // distance to far clipping plane
    /* 0x024 */ f32 scale; // scale for matrix elements
    /* 0x028 */ Vec3f eye;
    /* 0x034 */ Vec3f at;
    /* 0x040 */ Vec3f up;
    /* 0x050 */ Vp vp;
    /* 0x060 */ Mtx projection;
    /* 0x0A0 */ Mtx viewing;
    /* 0x0E0 */ Mtx* projectionPtr;
    /* 0x0E4 */ Mtx* viewingPtr;
    /* 0x0E8 */ Vec3f distortionOrientation;
    /* 0x0F4 */ Vec3f distortionScale;
    /* 0x100 */ f32 distortionSpeed;
    /* 0x104 */ Vec3f curDistortionOrientation;
    /* 0x110 */ Vec3f curDistortionScale;
    /* 0x11C */ u16 normal; // used to normalize the projection matrix
    /* 0x120 */ s32 flags;
    /* 0x124 */ s32 unk_124;
}
View;

ASSERT_SIZE(View, 0x128);

#endif
