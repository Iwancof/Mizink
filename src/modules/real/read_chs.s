read_chs:
; read_chs(drive_struct_address, read_sector[sect], dst[sect])
	push	bp
	mov	bp, sp

	push	3	; 試行回数
	push	0

	push	bx
	push	cx
	push	dx
	push	es
	push	si

	mov	si, [bp + 4]	; ドライブ構造体アドレス
	mov	ch, [si + drive.cyln + 0]
	mov	cl, [si + drive.cyln + 1]
	shl	cl, 6
	; clの下位2bitをchにつなげている
	or	cl, [si + drive.sect]
	; clの下位6bitはsectをセット

	mov	dh, [si + drive.head]
	mov	dl, [si + drive.no]
	
	mov	ax, 0x0000
	mov	es, ax

	mov	bx, [bp + 8]	; bx = dst
.10L:
	mov	ah, 0x02
	mov	al, [bp + 6]	; al = sector

	int	0x13		; BIOSコール
	jnc	.11E
	; エラーならCarryフラグがセットされる
	; https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=02h:_Read_Sectors_From_Drive

	mov	al, 0
	jmp	.10E
.11E:	; エラー
	cmp	al, 0
	jne	.10E

	dec	word [bp - 2]	; 試行回数
	jnz	.10L
	mov	ax, 0		; エラー
.10E:
	mov	ah, 0

	pop	si
	pop	es
	pop	dx
	pop	cx
	pop	bx
	
	mov	sp, bp
	pop	bp

	ret
