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
	pusha
	call	int_keyboard
	popa
	iret
