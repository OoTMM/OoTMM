#ifndef INCLUDED_MM_TYPES_H
#define INCLUDED_MM_TYPES_H

#include <combo/types.h>

typedef struct View {
    /* 0x000 */ u32 magic; /* string literal "VIEW" / 0x56494557 */
    /* 0x004 */ struct GraphicsContext* gfxCtx;
    /* 0x008 */ Viewport viewport;
    /* 0x018 */ f32 fovy;  /* vertical field of view in degrees */
    /* 0x01C */ f32 zNear; /* distance to near clipping plane */
    /* 0x020 */ f32 zFar;  /* distance to far clipping plane */
    /* 0x024 */ f32 scale; /* scale for matrix elements */
    /* 0x028 */ Vec3f eye;
    /* 0x034 */ Vec3f at;
    /* 0x040 */ Vec3f up;
    /* 0x04C */ u8 pad4C[0x4];
    /* 0x050 */ Vp vp;
    /* 0x060 */ Mtx projection;
    /* 0x0A0 */ Mtx viewing;
    /* 0x0E0 */ Mtx unkE0;
    /* 0x120 */ Mtx* projectionPtr;
    /* 0x124 */ Mtx* viewingPtr;
    /* 0x128 */ Vec3f distortionOrientation;
    /* 0x134 */ Vec3f distortionScale;
    /* 0x140 */ f32 distortionSpeed;
    /* 0x144 */ Vec3f curDistortionOrientation;
    /* 0x150 */ Vec3f curDistortionScale;
    /* 0x15C */ u16 perspNorm; /* used to normalize the projection matrix */
    /* 0x160 */ u32 flags;  /* bit 3: Render to an orthographic perspective */
    /* 0x164 */ s32 unk164;
} View; /* size = 0x168 */

#endif
