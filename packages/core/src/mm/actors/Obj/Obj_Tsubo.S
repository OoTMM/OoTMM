#include <combo.h>

/* Resize the actor */
PATCH_START 0x8092951c
.int 0x1a4
PATCH_END

/* Wrap the init function */
PATCH_START 0x80929520
.int ObjTsubo_InitWrapper
PATCH_END

/* Relocs */
PATCH_START 0x809298c4
.int 0
PATCH_END

/* Spawn Drop */
PATCH_START 0x80928a80
  jal ObjTsubo_SpawnShuffledDrop
PATCH_END

PATCH_START 0x80928b8c
  jal ObjTsubo_SpawnShuffledDrop
PATCH_END

PATCH_START 0x80928ff4
  jal ObjTsubo_SpawnShuffledDrop
PATCH_END

PATCH_START 0x80929098
  jal ObjTsubo_SpawnShuffledDrop
PATCH_END

/* Relocs */
PATCH_START 0x80929794
.int 0
PATCH_END

PATCH_START 0x809297bc
.int 0
PATCH_END

PATCH_START 0x8092982c
.int 0
PATCH_END

PATCH_START 0x8092984c
.int 0
PATCH_END

/* Spawn Drop (flexible) */
PATCH_START 0x80928b7c
  jal ObjTsubo_SpawnShuffledDropFlexible
PATCH_END

PATCH_START 0x80928fe4
  jal ObjTsubo_SpawnShuffledDropFlexible
PATCH_END

PATCH_START 0x80929088
  jal ObjTsubo_SpawnShuffledDropFlexible
PATCH_END

/* Relocs */
PATCH_START 0x809297b8
.int 0
PATCH_END

PATCH_START 0x80929828
.int 0
PATCH_END

PATCH_START 0x80929848
.int 0
PATCH_END
