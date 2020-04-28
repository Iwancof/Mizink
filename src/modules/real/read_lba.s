read_lba:
; read_lba(drive_param, lba, read_sector, dst_adr)
	push	bp
	mov	bp, sp
	push	si

	mov	si, [bp + 4] ; パラメタ
	mov	ax, [bp + 6] ; LBA

	cdecl	lba_chs, word si, .chs, word ax

	mov	al, [si + drive.no]
	mov	[.chs + drive.no], al
	; .chsにナンバーはセットしてやる(lba_chsではやってくれない)

	cdecl	read_chs, .chs, word [bp + 8], word [bp + 10]
	pop	si
	mov	sp, bp
	pop	bp
	ret
ALIGN	2
.chs:	times	drive_size	db	0
