get_font_adr:
; get_font_adr(dst_font_adr)
	push	bp
	mov	bp, sp
	
	push	si
	push	ax
	push	bx
	push	es
	push	bp

	mov	si, [bp + 4]
	mov	ax, 0x1130
	mov	bh, 0x06
	; 一文字当たりのバイト数
	int	0x10
	; 10H:11H -- "Change text mode character set"
	; ES:BPにフォントアドレスが入る

	mov	[si + 0], es	; セグメント
	mov	[si + 2], bp	; オフセット

	pop	bp
	pop	es
	pop	bx
	pop	ax
	pop	si
	
	mov	sp, bp
	pop	bp

	ret

	

