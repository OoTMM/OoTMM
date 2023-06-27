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

/* Skip slot 3 with the cursor - file select */
FileSelectButtonIndexIncr:
  addiu t5,t9,1
  li    t9,2
  bne   t5,t9,.LFileSelectButtonIndexIncrEnd
   nop
  addiu t5,1
.LFileSelectButtonIndexIncrEnd:
  jr ra
   sh t5,-0x35d6(at)

PATCH_START 0x8080c754
  jal FileSelectButtonIndexIncr
   nop
PATCH_END

FileSelectButtonIndexDecr:
  addiu t6,t5,-1
  li    t5,2
  bne   t6,t5,.LFileSelectButtonIndexDecrEnd
   nop
  addiu t6,-1
.LFileSelectButtonIndexDecrEnd:
  jr ra
   sh t6,-0x35d6(at)

PATCH_START 0x8080c724
  jal FileSelectButtonIndexDecr
   nop
PATCH_END

/* Skip slot 3 with the cursor - file copy */
FileCopyButtonIndexIncr:
  addiu t9,t8,1
  li    t8,2
  bne   t9,t8,.LFileCopyButtonIndexIncrEnd
   nop
  addiu t9,1
.LFileCopyButtonIndexIncrEnd:
  jr ra
   sh t9,-0x35d6(at)

PATCH_START 0x80803c9c
  jal FileCopyButtonIndexIncr
   nop
PATCH_END

FileCopyButtonIndexDecr:
  addiu t7,t6,-1
  li    t6,2
  bne   t7,t6,.LFileCopyButtonIndexDecrEnd
   nop
  addiu t7,-1
.LFileCopyButtonIndexDecrEnd:
  jr ra
   sh t7,-0x35d6(at)

PATCH_START 0x80803c68
  jal FileCopyButtonIndexDecr
   nop
PATCH_END

/* Skip slot 3 with the cursor - file copy 2 */
FileCopy2ButtonIndexIncr:
  addiu t9,t8,1

  /* Check for selected slot */
  lh t8,0x4a38(t0)
  bne t8,t9,.LFileCopy2ButtonIndexIncrCheck2
   nop
  addiu t9,1

.LFileCopy2ButtonIndexIncrCheck2:
  /* Check for slot 2 */
  li   t8,2
  bne  t9,t8,.LFileCopy2ButtonIndexIncrEnd
   nop
  addiu t9,1

.LFileCopy2ButtonIndexIncrEnd:
  jr ra
   sh t9,-0x35d6(at)

PATCH_START 0x8080428c
  jal FileCopy2ButtonIndexIncr
   nop
PATCH_END

FileCopy2ButtonIndexDecr:
  addiu t5,t4,-1
  li    t4,2
  bne   t5,t4,.LFileCopy2ButtonIndexDecrEnd
   nop
  addiu t5,-1
.LFileCopy2ButtonIndexDecrEnd:
  jr ra
   sh t5,-0x35d6(at)

PATCH_START 0x80804224
  jal FileCopy2ButtonIndexDecr
   nop
PATCH_END

/* Skip slot 3 with the cursor - file erase */
FileEraseButtonIndexIncr:
  addiu t3,t2,1
  li    t2,2
  bne   t3,t2,.LFileEraseButtonIndexIncrEnd
   nop
  addiu t3,1
.LFileEraseButtonIndexIncrEnd:
  jr ra
   sh t3,-0x35d6(at)

PATCH_START 0x808058bc
  jal FileEraseButtonIndexIncr
   nop
PATCH_END

FileEraseButtonIndexDecr:
  addiu t9,t8,-1
  li    t8,2
  bne   t9,t8,.LFileEraseButtonIndexDecrEnd
   nop
  addiu t9,-1
.LFileEraseButtonIndexDecrEnd:
  jr ra
   sh t9,-0x35d6(at)

PATCH_START 0x80805888
  jal FileEraseButtonIndexDecr
   nop
PATCH_END
