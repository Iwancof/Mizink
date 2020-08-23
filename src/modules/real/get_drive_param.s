get_drive_param:
; get_drive_param(drive_dst_adr)
	push	bp
	mov	bp, sp

	push	es
	push	bx
	push	cx
	push	dx
	push	si
	push	di

	mov	si, [bp + 4]

	mov	ax, 0
	mov	es, ax
	mov	di, ax

	mov	dl, [si + drive.no]
	; drive index
	mov	ah, 0x08
	; "read_drive_parameters"
	mov	cx, 0
	int	0x13
	; ES:DIを読む

	jc	.10F

	mov	ax, cx
	and	ax, 0b00111111
	mov	[si + drive.sect], ax
	; セクタ数
	shr	cl, 6
	ror	cx, 8	; CLとCHの交換
	add	cx, 1
	mov	[si + drive.cyln], cx
	; シリンダ数

	mov	bx, dx
	shr	bx, 8	; BX = DH
	add 	bx, 1
	mov	[si + drive.head], bx

	mov	ax, 1
	; 成功

	jmp	.10E
.10F:	; 失敗
	mov	ax, 0
.10E:
	pop	di
	pop	si
	pop	dx
	pop	cx
	pop	bx
	pop	es

	mov	sp, bp
	pop	bp

	ret
