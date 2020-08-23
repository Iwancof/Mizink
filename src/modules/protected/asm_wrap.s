.intel_syntax noprefix
.code32

.global rk_hlt
rk_hlt:
	hlt
	ret

.global set_idtr
set_idtr:
	push	ebp
	mov	ebp, esp

	nop
	nop
	nop
	mov	eax, [ebp + 8]
	nop
	nop

	lidt	[ebp + 8]

	mov	esp, ebp
	pop	ebp

	ret

.global asm_int_keyboard
asm_int_keyboard:
	pushad
	cli
	call	int_keyboard
	mov	dx, 0x20
	mov	al, 0x20
	out	dx, al
	sti
	popad
	iret


.global asm_int_timer
asm_int_timer:
	pusha
	cli
	call	int_timer
	mov	dx, 0x20
	mov	al, 0x20
	out	dx, al
	sti
	popa
	iret
