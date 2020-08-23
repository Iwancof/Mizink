lba_chs:
; lba_chs(drive_param, drive_adr, lba)
	push	bp
	mov	bp, sp
	push	si
	push	ax
	push	dx
	push	bx
	push	di

	mov	si, [bp + 4]
	mov	di, [bp + 6]

	mov	al, [si + drive.head]
	mul	byte [si + drive.sect]
	; AXはヘッド数*セクタ数=総セクタ数(シリンダ当たり)
	mov	bx, ax
	mov	dx, 0
	mov	ax, [bp + 8]
	; LBA
	div	bx
	; BXでわってやる。
	; 商(AX)はシリンダ数
	; あまり(DX)はヘッド数

	mov	[di + drive.cyln], ax
	mov	ax, dx
	div	byte [si + drive.sect]
	; 同じように、次はセクタ数で割ってやる
	; 商(AL)はヘッド数
	; あまり(AH)はセクタ数
	movzx	dx, ah
	inc	dx
	mov	ah, 0x00
	mov	[di + drive.head], ax
	; AH = 0より、AX = AL
	mov	[di + drive.sect], dx
	
	pop	di
	pop	bx
	pop	dx
	pop	ax
	pop	si

	mov	sp, bp
	pop	bp

	ret
