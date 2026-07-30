#include <combo.h>

/* Disable the saria LW cutscene */
PATCH_START(0x80056e80)
  li v0,1
PATCH_END

/* Disable the light arrow cutscene */
PATCH_START 0x80056ef0
  li v0,1
PATCH_END

/* Disable the desert colossus cutscene */
PATCH_START 0x80056de4
  li v0,1
PATCH_END

/* Disable the bongo-bongo escape cutscene */
PATCH_START 0x80056e4c
  li v0,1
PATCH_END

/* Disable the tower collapse cutscene */
PATCH_START 0x80056f3c
  li v0,1
PATCH_END

/* Disable some entrance cutscenes */
PATCH_START 0x800efd04
.short 0xfff
PATCH_END

PATCH_START 0x800efd0c
.short 0xfff
PATCH_END

PATCH_START 0x800efd14
.short 0xfff
PATCH_END

PATCH_START 0x800efd1c
.short 0xfff
PATCH_END

PATCH_START 0x800efd24
.short 0xfff
PATCH_END

PATCH_START 0x800efd2c
.short 0xfff
PATCH_END

PATCH_START 0x800efd34
.short 0xfff
PATCH_END

PATCH_START 0x800efd3c
.short 0xfff
PATCH_END

PATCH_START 0x800efd6c
.short 0xfff
PATCH_END

PATCH_START 0x800efd74
.short 0xfff
PATCH_END

PATCH_START 0x800efd7c
.short 0xfff
PATCH_END

PATCH_START 0x800efd84
.short 0xfff
PATCH_END

PATCH_START 0x800efd8c
.short 0xfff
PATCH_END

PATCH_START 0x800efd94
.short 0xfff
PATCH_END

PATCH_START 0x800efd9c
.short 0xfff
PATCH_END

PATCH_START 0x800efda4
.short 0xfff
PATCH_END

PATCH_START 0x800efdac
.short 0xfff
PATCH_END

PATCH_START 0x800efdb4
.short 0xfff
PATCH_END

PATCH_START 0x800efdf4
.short 0xfff
PATCH_END

PATCH_START 0x800efdfc
.short 0xfff
PATCH_END

PATCH_START 0x800efe04
.short 0xfff
PATCH_END

.globl _OnePointCutscene_Init
_OnePointCutscene_Init:
  addiu sp, -0x48
  j OnePointCutscene_Init+0x8
   sw s3, 0x28(sp)

PATCH_START OnePointCutscene_Init
  j OnePointCutscene_InitHook
   nop
PATCH_END

.globl _OnePointCutscene_EndCutscene
_OnePointCutscene_EndCutscene:
  sw a1, 0x04(sp)
  j OnePointCutscene_EndCutscene+0x8
   sll a1, a1, 0x10

PATCH_START OnePointCutscene_EndCutscene
  j OnePointCutscene_EndCutsceneHook
   nop
PATCH_END
