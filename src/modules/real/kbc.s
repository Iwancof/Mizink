KBC_Data_Write:
; KBC_Data_Write(data)
	push	bp
	mov	bp, sp
	push	cx
	mov	cx, 0
	; CXを減算しながら処理するため、0 - 1 = 0xFFFF
.10L:
	in	al, 0x64
	; 0x64の読み込みはステータス取得
	test	al, 0x02
	; システムフラグ
	; ステータスレジスタの詳細はp293
	loopnz	.10L
	; CXが0ならタイムアウト
	; ZFが0ならデータ書き込む可能

	cmp	cx, 0
	je	.20E
	; タイムアウト

	mov	al, [bp + 4]	; 書き込むデータ
	out	0x60, al
.20E:
	mov	ax, cx		; リザルト(0ならタイムアウト)
	pop	cx
	mov	sp, bp
	pop	bp
	ret
KBC_Cmd_Write:
; KBC_Cmd_Write(cmd_data)
	push	bp
	mov	bp, sp
	push	cx

	mov	cx, 0	; 上に同じ
.10L:
	in	al, 0x64
	test	al, 0x02
	loopnz	.10L

	cmp	cx, 0
	jz	.20E

	mov	al, [bp + 4]
	out	0x64, al
	; 0x64の書き込みはコマンド
.20E:
	mov	ax, cx
	pop	cx
	mov	sp, bp
	pop	bp
	ret

KBC_Data_Read:
; KBC_Data_Read(data_adr)
	push	bp
	mov	bp, sp
	push	cx
	mov	cx, 0
.10L:
	in	al, 0x64
	test	al, 0x01
	; 0x01は入力バッファフルを確認
	loopz	.10L

	cmp	cx, 0
	jz	.20E

	mov	ah, 0x00
	in	al, 0x60
	; 0x60(data)の読み込み

	mov	di, [bp + 4]
	mov	[di + 0], ax
.20E:
	mov	ax, cx
	pop	cx
	mov	sp, bp
	pop	bp
	ret

