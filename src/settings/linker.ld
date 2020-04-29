
ENTRY(_start)
OUTPUT_FORMAT("binary")
/* _GLOBAL_OFFSET_TABLE(0) */


SECTIONS {
	.head	0x00010000:
	AT(0) {
	 	LONG(_start)
	}

	.text 0x0010000 + SIZEOF(.head):
	AT(SIZEOF(.head)) SUBALIGN(4) {
		*(.text*)
	}
	.data 0x00010000 + LOADADDR(.text) + SIZEOF(.text):
	AT(LOADADDR(.text) + SIZEOF(.text)) SUBALIGN(4) {
	  *(.data*)
	  *(.rodata*)
	}

	.bss 0x00010000 + LOADADDR(.data) + SIZEOF(.data):
	AT(LOADADDR(.data) + SIZEOF(.data)) SUBALIGN(4) {
	  *(.bss*)
	}
	/*
	.plt 0x00010000 + LOADADDR(.bss) + SIZEOF(.bss):
	AT(LOADADDR(.bss) + SIZEOF(.bss)) SUBALIGN(4) {
	  *(.plt*)
	}
	  */
	/*
	.else 0x00010000 + LOADADDR(.bss) + SIZEOF(.bss):
	AT(LOADADDR(.bss) + SIZEOF(.bss)) SUBALIGN(4) {
	  *(*)
	}
	  */
	
	/DISCARD/ : {
	  *(*)
	}
}
