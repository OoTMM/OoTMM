#include <combo.h>

/* Patch the schedule to check for room key and kafei's letter in the xitem */
PATCH_START 0x801c5c70
.int Schedule_CheckMiscSWrapper
PATCH_END

PATCH_START 0x80132994
  li at,1
  lui t7,%HI(gScheduleXitemFlags)
  lbu t7,%LO(gScheduleXitemFlags)(t7)
  and t7,at
PATCH_END

PATCH_START 0x801329b8
  li at,2
  lui t9,%HI(gScheduleXitemFlags)
  lbu t9,%LO(gScheduleXitemFlags)(t9)
  and t9,at
PATCH_END
