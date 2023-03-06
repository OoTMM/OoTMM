#include <combo.h>

/* Enlarge the actor */
PATCH_START 0x80864a3c
.int 0x1d0
PATCH_END

EnGirlA_PostHandler_Trampoline:
  /* girlA is in s0 */
  move a0,s0

  /* play is on the stack */
  lw a1,0x34(sp)

  /* Replaced code */
  lw    s0,0x18(sp)
  addiu sp,0x30

  /* Call our code */
  j EnGirlA_PostHandler
   nop

/* Use our own draw func */
PATCH_START 0x808649c8
  j EnGirlA_Draw
   nop
PATCH_END

/* Hook after EnGirlA_Handler */
PATCH_START 0x808648e8
  j EnGirlA_PostHandler_Trampoline
   nop
PATCH_END

/* Always allow the Bomb Bag to be bought */
PATCH_START 0x80863d8c
  li at,1
PATCH_END

PATCH_START 0x80863df0
  li t7,0
PATCH_END

PATCH_START 0x80863e08
  li at,1
PATCH_END

PATCH_START 0x80863e70
  li t7,0
PATCH_END

PATCH_START 0x80863e84
  li t9,0
PATCH_END
