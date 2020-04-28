# 定数変換用スクリプトコンパイル
cd settings
csc constant_converter.cs
./constant_converter.exe
cd ..

nasm boot.s -o boot.bin -l lists/boot.lst

# main kernel
wsl gcc cmain.c -nostdlib -nostdinc -c -O0 -g -m32 -o cmain.o -masm=intel

# modules
cd 
wsl gcc modules/protected/out.s -c -g -m32 -o kernel_asm_modules.o -O0 -nostdlib -nostdinc

wsl i686-unknown-linux-gnu-ld cmain.o kernel_asm_modules.o -T settings/linker.ld -o cmain.bin

$padding = 0x6000
$padding -= (Get-Item cmain.bin).Length;

fsutil file createnew padding.bin $padding

cmd /C copy /B boot.bin+cmain.bin+padding.bin Mizink.img

#clear up
rm cmain.o
rm boot.bin
rm padding.bin
# rm cmain.bin
rm settings/define.s
rm settings/define.c



