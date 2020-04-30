#include "../kernel_modules.h"

.intel_syntax noprefix
.code32

.global outb
outb:
	push	ebp
	mov	ebp, esp
	
	push	eax
	push	ebx

	mov	dx, [ebp + 8]
	mov	al, [ebp +12]
	out	dx, al
	
	pop	eax
	pop	ebx

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
	
	push	eax
	push	ebx

	mov	dx,  [ebp + 8]
	mov	eax, [ebp +12]
	out	dx, eax
	
	pop	ebx
	pop	eax

	mov	esp, ebp
	pop	ebp

	ret

.global inb
inb:
	push	ebp
	mov	ebp, esp

	push	edx
	mov	eax, 0

	mov	dx, [ebp + 8]
	in	al, dx

	pop	edx
	mov	esp, ebp
	pop	ebp

	ret

.global inw
inw:
	push	ebp
	mov	ebp, esp

	push	edx
	mov	eax, 0

	mov	dx, [ebp + 8]
	in	ax, dx

	pop	edx
	mov	esp, ebp
	pop	ebp

	ret

.global ind
ind:
	push	ebp
	mov	ebp, esp

	push	edx
	mov	eax, 0

	mov	edx, [ebp + 8]
	in	eax, dx

	pop	edx
	mov	esp, ebp
	pop	ebp

	ret

