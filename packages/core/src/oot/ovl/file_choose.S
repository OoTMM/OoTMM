#include <combo.h>

.global hookDeep_FileChoose_Update
hookDeep_FileChoose_Update:
  HOOK_CALL(hook_FileChoose_Update)

  /* Original code */
  lh  t3,0x4a2e(a2)
  sll t4,t3,0x2

  /* Jump back */
  jr      ra
   nop

PATCH_START(0x80811ab8)
  jal hookDeep_FileChoose_Update
   nop
PATCH_END

/* Patch the FileChoose_Init function to mute BGM when coming from MM */
PATCH_START(0x808124a8)
  jal hook_FileChoose_Init
PATCH_END

/* Hide slot 3 */
PATCH_START 0x8080f178
  slti at,s3,0x2
PATCH_END

HideSlot3Hook1:
  /* Increment tmp by 20 */
  lh t3,0x274(sp)
  addiu t3,0x14
  sh t3,0x274(sp)

  /* Replaced code */
  jr ra
   lui t3,0x102

PATCH_START 0x8080f188
  jal HideSlot3Hook1
PATCH_END

PATCH_START 0x8080f708
  slti at,t4,0x2
PATCH_END

HideSlot3Hook2:
  /* Increment tmp by 20 */
  lh s3,0x274(sp)
  addiu s3,0x14
  sh s3,0x274(sp)

  /* Replaced code */
  jr ra
   move s3,zero

PATCH_START 0x8080f718
  jal HideSlot3Hook2
PATCH_END

PATCH_START 0x8080f778
  slti at,s3,0x2
PATCH_END
