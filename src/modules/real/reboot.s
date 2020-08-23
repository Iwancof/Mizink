reboot:
	cdecl	puts, .s0

.10L:
	mov	ah, 0x10
	int	0x16
	; 16H:10H キー入力。リザルトはAL

	cmp	al, ' '
	jne	.10L

	int	0x19

.s0:	db	0x0A, 0x0D, "PUSH SPACE KEY TO REBOOT", 0x0A, 0x0D, 0
