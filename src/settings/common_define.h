
#define	SECT_SIZE	(512)
#define	E820_RECORD_SIZE	(20)


#define	VECT_BASE	(BOOT_END-0x1000)


#define	BOOT_LOAD	(0x7C00)
#define	BOOT_STACK	(BOOT_LOAD)
#define	BOOT_SIZE	(1024*8)
#define	BOOT_SECT	(BOOT_SIZE/SECT_SIZE)
#define	BOOT_END	(BOOT_LOAD+BOOT_SIZE)

#define	KERNEL_LOAD	(0x00010000)
#define	KERNEL_SIZE	(0x00006000)
#define	KERNEL_SECT	(KERNEL_SIZE/SECT_SIZE)
#define	KERNEL_END	(KERNEL_LOAD+KERNEL_SIZE)

#define	VGA_SEQ_ADR	(0x03C4)
#define	VGA_SEQ_DAT	(0x03C5)
#define	VGA_GRA_ADR	(0x03CE)
#define	VGA_GRA_DAT	(0x03CF)

#define	HEAP_START	(0x00030000)

#define	VGA_IO_MEM_BASE	(((char*)0xA0000))



