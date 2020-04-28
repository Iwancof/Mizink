.intel_syntax noprefix
.code32

.global outb
outb:
	push	ebp
	mov	ebp, esp

	mov	dx, [ebp + 8]
	mov	al, [ebp +12]
	out	dx, al

	mov	esp, ebp
	pop	ebp

	ret

.global outw
outw:
	push	ebp
	mov	ebp, esp
	
	push	eax
	push	edx

	mov	dx, [ebp + 8]
	mov	ax, [ebp +12]
	out	dx, ax

	pop	edx
	pop	eax

	mov	esp, ebp
	pop	ebp

	ret

.global outd
outd:
	push	ebp
	mov	ebp, esp

	mov	dx,  [ebp + 8]
	mov	eax, [ebp +12]
	out	dx, eax

	mov	esp, ebp
	pop	ebp

	ret

