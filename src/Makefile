
.PHONY: all

MODULES	=	modules
PROTMOD	=	$(MODULES)/protected
LISTINGFILE =	lists
SETTINGSFILE =	settings


PROTECTED_MODULE_CFLAGS	=	-O0	\
				-c -g -m32	\
				-nostdlib -nostdinc -fno-builtin -fno-PIC -msoft-float -mno-sse -lsoft-fp

KERNEL_CFLAGS		=	-O0	\
				-c -g -m32	\
				-nostdlib -nostdinc -fno-builtin -fno-PIC -msoft-float -mno-sse -lsoft-fp

KERNEL_RUSTFLAGS	=	--crate-type=staticlib -C lto -C opt-level=0 -C no-prepopulate-passes -Z verbose --target=i686-unknown-linux-gnu --emit=obj -Ctarget-feature=+soft-float -C relocation-model=dynamic-no-pic

PROTECTED_MODULES	=	out.o			\
				vga.o			\
				draw.o			\
				asm_wrap.o		\
				kernel_panic.o		\
				interrupt.o		\
				keyboard.o		\
				timer.o			\
				calc_mod.o		\
				memory_ope.o		\
				interrupt_task.o	\
				vector.o		\
				string.o		\

ASM_BOOT_MAIN		=	boot.s
C_KERNEL_MAIN		=	cmain.c
RUST_KERNEL_MAIN	=	ul.rs

BIN	=	clean	\
		Mizink

all: $(BIN)

clean:
	rm *.o *.bin -f

boot.bin: $(ASM_BOOT_MAIN)
	nasm	-o $@ $(ASM_BOOT_MAIN) -l $(LISTINGFILE)/$*.lst

cmain.o: $(C_KERNEL_MAIN)
	gcc	-o $@ $(C_KERNEL_MAIN) $(KERNEL_CFLAGS) 

rmain.o: $(RUST_KERNEL_MAIN)
	rustc	-o $@ $(RUST_KERNEL_MAIN) $(KERNEL_RUSTFLAGS)

out.o:			
	gcc	-o $@ $(PROTMOD)/$*.s $(PROTECTED_MODULE_CFLAGS) 
vga.o:			
	gcc	-o $@ $(PROTMOD)/$*.c $(PROTECTED_MODULE_CFLAGS) 
draw.o:			
	gcc	-o $@ $(PROTMOD)/$*.c $(PROTECTED_MODULE_CFLAGS) 
asm_wrap.o:		
	gcc	-o $@ $(PROTMOD)/$*.s $(PROTECTED_MODULE_CFLAGS) 
kernel_panic.o:		
	gcc	-o $@ $(PROTMOD)/$*.c $(PROTECTED_MODULE_CFLAGS) 
interrupt.o:		
	gcc	-o $@ $(PROTMOD)/$*.c $(PROTECTED_MODULE_CFLAGS) 
keyboard.o:		
	gcc	-o $@ $(PROTMOD)/$*.c $(PROTECTED_MODULE_CFLAGS) 
timer.o:		
	gcc	-o $@ $(PROTMOD)/$*.c $(PROTECTED_MODULE_CFLAGS) 
calc_mod.o:		
	gcc	-o $@ $(PROTMOD)/$*.c $(PROTECTED_MODULE_CFLAGS) 
memory_ope.o:		
	gcc	-o $@ $(PROTMOD)/$*.c $(PROTECTED_MODULE_CFLAGS) 
interrupt_task.o:	
	gcc	-o $@ $(PROTMOD)/$*.c $(PROTECTED_MODULE_CFLAGS) 
vector.o:		
	gcc	-o $@ $(PROTMOD)/$*.c $(PROTECTED_MODULE_CFLAGS) 
string.o:		
	gcc	-o $@ $(PROTMOD)/$*.c $(PROTECTED_MODULE_CFLAGS) 
c_kernel.bin:	$(PROTECTED_MODULES) cmain.o rmain.o
	i686-unknown-linux-gnu-ld -o $@  -T $(SETTINGSFILE)/linker.ld cmain.o rmain.o $(PROTECTED_MODULES)

Mizink:	c_kernel.bin boot.bin
	touch Mizink.img
	truncate -s 24576 c_kernel.bin
	cat boot.bin c_kernel.bin > Mizink.img
	
	

