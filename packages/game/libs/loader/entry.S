.set noreorder

.balign 16

.section .text
.global entrypoint
entrypoint:
  la $sp,gLoaderStack + 0x400
  la $t2,LoaderInit
  jr $t2
   nop
