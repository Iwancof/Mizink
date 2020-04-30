# 定数変換用スクリプトコンパイル
cd settings
# csc constant_converter.cs
./constant_converter.exe
cd ..

nasm boot.s -o boot.bin -l lists/boot.lst

# main kernel
wsl gcc cmain.c -nostdlib -nostdinc -c -O0 -g -m32 -o cmain.o -masm=intel

# user land in rust
rustc .\ul.rs --crate-type=staticlib -C lto -C opt-level=0 -C no-prepopulate-passes -Z verbose -Z no-landing-pads --target=i686-unknown-linux-gnu -o .\rmain.o --emit=obj -Ctarget-feature=+soft-float -C relocation-model=dynamic-no-pic

$list = New-Object `System.Collections.Generic.List[string]`;

# modules
$list.Add("modules/protected/out.s");
$list.Add("modules/protected/vga.c");
$list.Add("modules/protected/draw.c");
$list.Add("modules/protected/asm_wrap.s");
$list.Add("modules/protected/kernel_panic.c");
$list.Add("modules/protected/interrupt.c");
$list.Add("modules/protected/keyboard.c");

$index = 0;
$cmd = "wsl i686-unknown-linux-gnu-ld -T settings/linker.ld -o cmain.bin rmain.o cmain.o ";
foreach($e in $list) {
  wsl gcc $e -c -g -m32 -o obj_$index.o -O0 -nostdlib -nostdinc
  $cmd += "obj_" + $index.ToString() + ".o ";
  $index++;
}
Invoke-Expression $cmd

$padding = 0x6000
$padding -= (Get-Item cmain.bin).Length;

fsutil file createnew padding.bin $padding

cmd /C copy /B boot.bin+cmain.bin+padding.bin Mizink.img

#clear up
# rm cmain.o
# rm rmain.o
rm boot.bin
rm padding.bin
rm cmain.bin
rm settings/common_define.s
rm settings/common_define.h
# rm obj_*.o


