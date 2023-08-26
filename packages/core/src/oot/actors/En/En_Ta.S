#include <combo.h>

/*
    File:   Talon - Actor
    Start:  0x80921830 - 0xcbefa0	
    End:    0x809251f0 - 0xcc2960
    Size:   0x39c0
*/


talon_break_free:

  //displaced code
  addiu    t1, zero, 0x0041

  //preserve registers (t0, t1, t2, t4)
  addiu    sp, sp, -0x20
  sw       t0, 0x04(sp)
  sw       t1, 0x08(sp)
  sw       t2, 0x0C(sp)
  sw       t4, 0x10(sp)

  lui    t2, 0xFFFF
  sra    t2, t2, 0x10   //shift right 2 bytes
  lui    t0, 0x801D
  lh     t4, 0x894C(t0) //load current value @z64_game.event_flag
  beq    t4, t2, msg  //if in non-cs state, jump to next check
  nop
  sh     zero, -0x76B4(t0) //store 0 to z64_game.event_flag

msg:
  lui    t0, 0x801E
  lb     t2, 0x887C(t0)
  beqz   t2, return   //return if msg_state_1 is 0
  nop
  lui    t1, 0x0036
  sra    t1, t1, 0x10   //shift right 2 bytes
  sb     t1, -0x7784(t0)   //store 0x36 to msg_state_1
  lui    t1, 0x0002
  sra    t1, 0x10       //shift right 2 bytes
  sb     t1, -0x76a1(t0)   //store 0x02 to msg_state_3
  lui    t0, 0x801F
  sb     zero, -0x72c8(t0) //store 0x00 to msg_state_2

return:
  lw       t4, 0x10(sp)
  lw       t2, 0x0C(sp)
  lw       t1, 0x08(sp)
  lw       t0, 0x04(sp)
  jr     ra
  addiu    sp, sp, 0x20

PATCH_VROM 0x80921830 + 0x1098
  jal    talon_break_free
  lw     a0, 0x0018(sp)
PATCH_END