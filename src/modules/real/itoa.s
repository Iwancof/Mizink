itoa:
; itoa(num, buf, buf_size, radix, flag)
	push	bp
	mov	bp, sp

	pusha

	mov	bx, word [bp + 12]
	mov	cx, word [bp + 8]
	mov	si, word [bp + 6]

	mov	di, si
.10A:
	mov	al, ' '
	test	bx, 0b0100
	je	.42E
	mov	al, '0'
.42E:
	cld
	rep	stosb
	; ' 'で埋める

	mov	ax, [bp + 4]
	mov	cx, [bp + 8]
	mov	di, si
	add	di, cx
	; DI = BUF + size. バッファの最後
	dec	di

	test	bx, 0b0001	; 符号付きか
	je	.10E
	cmp	ax, 0
	jge	.10E		; 符号付きで負
	or	bx, 0b0010	; 符号付きのビットを立てる
.10E:
	test	bx, 0b0010
	jz	.20E		; 符号を描画しない->20E
	cmp	ax, 0
	jge	.22F
	neg	ax
	mov	[si], byte '-'
	jmp	.22E
.22F:
	mov	[si], byte '+'
.22E:
	dec	cx
.20E:
	mov	bx, [bp + 10]
.30L:
	mov	dx, 0
	div	bx
	; AXをBX(radix)で割る
	; DX -> あまり, AX -> 商

	mov	si, dx
	mov	dl, byte [.ascii + si]

	mov	[di], dl
	dec	di

	cmp	ax, 0
	loopnz	.30L
.40A:
	popa

	mov	sp, bp
	pop	bp

	ret

.ascii:	db	"0123456789ABCDEF"




