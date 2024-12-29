.set noat
.set noreorder

.section .text, "ax"

.word 0x80371240
.word 0x0000000f
.word entrypoint
.word 0x1449
.word 0,0,0,0
.ascii "OOT+MM COMBO       "
.byte 0,0,0,0,0,0,0,0
.byte 'C'
.ascii "ZLE"
.byte 0

.incbin "oot/extracted/ntsc-1.0/incbin/ipl3"
