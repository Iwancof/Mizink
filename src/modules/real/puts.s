puts:
; puts(string_address)
; draw until 0
	push	bp
	mov	bp, sp

	push	ax
	push	bx
	push	si

	mov	si, [bp + 4]

	mov	ah, 0x0E
	; 文字描画モード
	mov	bx, 0x0000
	; 色

	cld	; 加算方向を正に
.10L:
	lodsb
	; AL =* SI; SI++
	cmp	al, 0
	je	.10E

	int	0x10
	jmp	.10L
	; まだ描画
.10E:
	pop	si
	pop	bx
	pop	ax

	mov	sp, bp
	pop	bp

	ret
