.set noreorder

#define BOOT_STACK_SIZE 0x400

.balign 16

.section .text.entry, "ax"
.global entrypoint
entrypoint:
    /* Clear boot BSS */
    la      $t0, _bootSegmentBssStart
    la      $t1, _bootSegmentBssSize

.clear_bss:
    addi    $t1, $t1, -8
    sw      $zero, ($t0)
    sw      $zero, 4($t0)
    bnez    $t1, .clear_bss
     addi   $t0, $t0, 8
    
    /* Prepare for execution */
    lui     $t2, %hi(bootproc)
    lui     $sp, %hi(sBootThreadStack + BOOT_STACK_SIZE)
    addiu   $t2, %lo(bootproc)
    jr      $t2
     addiu  $sp, %lo(sBootThreadStack + BOOT_STACK_SIZE)

.fill 0x60 - (. - entrypoint)
