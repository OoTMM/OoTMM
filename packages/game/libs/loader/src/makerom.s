.set noat
.set noreorder

.section .makerom, "ax"
.word 0x80371240
.word 0x0000000f
.word _LoaderEntry
.word 0x1449
.word 0,0,0,0
.ascii "THE LEGEND OF ZELDA"
.byte 0,0,0,0,0,0,0
.byte 'C'
.ascii "ZLE"
.byte 0
