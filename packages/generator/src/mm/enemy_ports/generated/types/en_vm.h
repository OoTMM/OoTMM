#ifndef Z_EN_VM_H
#define Z_EN_VM_H

struct EnVm;

typedef void (*EnVmActionFunc)(struct EnVm*, struct PlayState*);

typedef struct EnVm {
    Actor actor;
    SkelAnime skelAnime;
    Vec3s jointTable[11];
    Vec3s morphTable[11];
    EnVmActionFunc actionFunc;
    f32 beamSightRange;
    s32 unk_21C;
    s32 timer;
    Vec3f beamPos1;
    Vec3f beamPos2;
    Vec3f beamPos3;
    Vec3f beamScale;
    Vec3s beamRot;
    s16 beamTexScroll;
    s16 headRotY;
    s16 unk_25E;
    s16 unk_260;
    f32 beamSpeed;
    ColliderCylinder colliderCylinder;
    ColliderQuad colliderQuad1;
    ColliderQuad colliderQuad2;
} EnVm;

typedef enum BeamosType {
    BEAMOS_LARGE,
    BEAMOS_SMALL
} BeamosType;

#endif
