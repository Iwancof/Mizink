; MizinkOS
; Written by @Iwancof_ptr in 2020/4/28

%include	"settings/common_define.s"
%include	"settings/macro.s"
%include	"settings/structs.s"

ORG	BOOT_LOAD

boot_entry:
	jmp	ipl
	times	3 - ($ - $$) db	0x90
	db	'OEM-NAME'
	
	dw	512
	db	1
	dw	32
	db	2
	dw	512
	dw	0xFFF0
	db	0xF8
	dw	256
	dw	0x10
	dw	2
	dd	0

	dd	0
	db	0x80
	db	0
	db	0x29
	dd	0xBEEF
	db	'BOOTABLE   '
	db	'FAT16   '

ipl:	cli	; 割り込み禁止

	mov	ax, 0x0000	; ディスクリプタ
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, BOOT_STACK
	; 各種セグメントレジスタを初期化
	
	sti
	mov	[BOOT_DRIVE + drive.no], dl
	; dl にはドライブの番号がセットされている
	
	mov	bx, BOOT_SECT - 1
	; 何セクタ読むか
	mov	cx, BOOT_LOAD + SECT_SIZE
	; どこから　読むか
	cdecl	read_chs, BOOT_DRIVE, bx, cx
	; AX = (呼んだセクタ数)

	cmp	ax, bx
	je	.10E	; 成功
	
	cdecl	puts, .e0
	call	reboot
.10E:
	jmp	main_boot_program	

		
.s0	db	"Real mode started", 0x0D, 0x0A, 0, 
.e0	db	"Could not read stage2", 0

ALIGN	2, 	db	0
BOOT_DRIVE:	
	istruc drive
	; 初期値(最初のread_chsで使用)
	; sectが2になっているのは2セクタ目から読み込みたいから
		at drive.no,	dw	0
		at drive.cyln,	dw	0
		at drive.head,	dw	0
		at drive.sect,	dw	2
	iend

%include	"modules/real/read_chs.s"
%include	"modules/real/puts.s"
%include	"modules/real/reboot.s"

	times	510 - ($ - $$)	db	0x00
	db	0x55, 0xAA

FONT:
.seg:	dw	0
.off:	dw	0
ACPI_DATA:
.adr:	dd	0
.len:	dd	0

%include	"modules/real/get_drive_param.s"
%include	"modules/real/get_font_adr.s"
%include	"modules/real/itoa.s"
%include	"modules/real/get_mem_info.s"
%include	"modules/real/kbc.s"
%include	"modules/real/lba_chs.s"
%include	"modules/real/read_lba.s"

main_boot_program:
	cdecl	get_drive_param, BOOT_DRIVE
	; BOOT_DRIVEに使っているドライブの情報を入れる
	cmp	ax, 0
	jne	.10E	; 成功
	cdecl	puts, errmsg_read_drive
	call	reboot
.10E:
	cdecl	get_font_adr, FONT
	; BISOのフォント取得(Protected-Modeでも使えるように)
	cdecl	get_mem_info
	; RAMに配置されている情報を得る

	mov	eax, [ACPI_DATA.adr]
	cmp	eax, 0
	je	.20E
	; ACPI利用不可
	cdecl	puts, msg_able_to_use_acpi
.20E:
	; A20 有効化(KBCを使う)
	cli	; まず割り込み禁止
	cdecl	KBC_Cmd_Write, 0xAD
	; キーボードインターフェース無効
	cdecl	KBC_Cmd_Write, 0xD0
	; 出力ポート読み出し
	cdecl	KBC_Data_Read, kbc_register_key

	mov	bl, [kbc_register_key]
	or	bl, 0x02
	; KBCのレジスタのB2はA20有効化ビットになっている
	cdecl	KBC_Cmd_Write, 0xD1
	; 書き込みモード
	cdecl	KBC_Data_Write, bx
	; A20書き込み
	cdecl	KBC_Cmd_Write, 0xAE
	; 再有効化
	sti

	mov	ax, 0x0012
	int	0x10
	; 10H:12H ビデオモードの変更。グラフィックモードにする。

	; カーネルの読み込み処理
	cdecl	read_lba, BOOT_DRIVE, BOOT_SECT, KERNEL_SECT, BOOT_END
	cmp	ax, KERNEL_SECT
	jz	.30E
	cdecl	puts, errmsg_read_kernel
	call	reboot
.30E:
	; カーネルの読み込みが完了
	; Protected-Modeへの移行を開始

	cli
	lgdt	[GDTR]	; GDTRをセット
	lidt	[IDTR]
	mov	eax, cr0
	; 制御レジスタを取得
	or	eax, 1
	; Protected-Mode有効化ビットを立てる
	mov	cr0, eax

	jmp	$ + 2
	; パイプラインの内容を破棄

[BITS 32]	; 32bitモードの機械語の出力を指示
	db	0x66
	; Real-Modeでe*xレジスタの使用を可能にするオーバーライドプレフィックス
	jmp	SEL_CODE:CODE_32
	; ipを参照するときSEL_CODEをセグメントディスクリプタにする
CODE_32:
	mov	ax, SEL_DATA
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	ss, ax
	; GS,SSはESのさらなる追加

	lea	eax, [default_panic]
	mov	ebx, 0x00088E00
	xchg	ax, bx

	mov	ecx, 256
	mov	edi, VECT_BASE
.40L:
	mov	[edi + 0], ebx
	mov	[edi + 4], eax
	add	edi, 8
	loop	.40L

	lidt	[BOOT_END - 0x10]

	; BOOT_ENDに追加されたメモリをKERNEL領域に書き込む
	mov	ecx, (KERNEL_SIZE) / 4
	; バイト数を取得
	mov	esi, BOOT_END
	; どこからコピーするか
	mov	edi, KERNEL_LOAD
	cld
	rep	movsd

	jmp	[KERNEL_LOAD]

s0:	db	"--------", 0
s1:	db	"--------", 0

default_panic:
	jmp	$


ALIGN	4,	db	0
; 一時的なGDTを作成
GDT:	dq	0x00_0000_000000_0000	; NULL
.cs:	dq	0x00_CF9A_000000_FFFF	; CODE(4G)
.ds:	dq	0x00_CF92_000000_FFFF	; DATA(4G)
.gdt_end:

; 各ディスクリプタのセレクタ(GDTからのオフセット)
SEL_CODE	equ	GDT.cs - GDT
SEL_DATA	equ	GDT.ds - GDT

	
suc:			db	"success", 0
msg_able_to_use_acpi	db	"ACPI available", 0x0D, 0x0A, 0

errmsg_read_drive:	db	"Could not get drive parameter.", 0,
errmsg_read_kernel	db	"Could not load kernel binary", 0
kbc_register_key:	dw	0

times	BOOT_SIZE - 0x1000 - ($ - $$)	db	0

times	BOOT_SIZE - 0x20 - ($ - $$)	db	0
; リミッタとアドレス
GDTR:	dw	GDT.gdt_end - GDT - 1
	dd	GDT
times	BOOT_SIZE - 0x10 - ($ - $$)	db	0
; 
IDTR:	dw	256 * 8 - 1
	dd	VECT_BASE


times	BOOT_SIZE - ($ - $$)	db	0

; Memory Map
;+-------------+0
;|interrupt+etc|
;+-------------+??
;|none or stack|
;+-------------+07C00
;|boot program |
;+-------------+09C00
;| copy kernel |
;+-------------+0FC00
;|   not use   |
;+-------------+10000
;| kernel code |
;+-------------+
