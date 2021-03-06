SECT_SIZE	equ	512
E820_RECORD_SIZE	equ	20
VECT_BASE	equ	BOOT_END-0x1000
BOOT_LOAD	equ	0x7C00
BOOT_STACK	equ	BOOT_LOAD
BOOT_SIZE	equ	1024*8
BOOT_SECT	equ	BOOT_SIZE/SECT_SIZE
BOOT_END	equ	BOOT_LOAD+BOOT_SIZE
KERNEL_LOAD	equ	0x00010000
KERNEL_SIZE	equ	0x00006000
KERNEL_SECT	equ	KERNEL_SIZE/SECT_SIZE
KERNEL_END	equ	KERNEL_LOAD+KERNEL_SIZE
VGA_SEQ_ADR	equ	0x03C4
VGA_SEQ_DAT	equ	0x03C5
VGA_GRA_ADR	equ	0x03CE
VGA_GRA_DAT	equ	0x03CF
HEAP_START	equ	0x00030000
