nasm boot.s -o boot.bin -l lists/boot.lst

wsl gcc cmain.c -nostdlib -nostdinc -c -O0 -g -m32 -o cmain.o
wsl i686-unknown-linux-gnu-ld cmain.o -T settings/linker.ld -o cmain.bin

$padding = 0x6000
$padding -= (Get-Item cmain.bin).Length;

fsutil file createnew padding.bin $padding

cmd /C copy /B boot.bin+cmain.bin+padding.bin Mizink.img

#clear up
rm cmain.o
rm boot.bin
rm padding.bin
rm cmain.bin




