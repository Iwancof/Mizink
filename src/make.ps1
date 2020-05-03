# 定数変換用スクリプトコンパイル
cd settings
# csc constant_converter.cs
./constant_converter.exe
cd ..

nasm boot.s -o boot.bin -l lists/boot.lst

# c-opt-level
$c_opt = 3;

# rs-opt-level
$rs_opt = 3;

# main kernel
$comp_cmain = "wsl gcc cmain.c -nostdlib -nostdinc -c -O" + $c_opt.ToString() + " -g -m32 -o cmain.o -masm=intel -fno-builtin"
Invoke-Expression $comp_cmain;

# user land in rust
rustc .\ul.rs --crate-type=staticlib -C lto -C opt-level=$rs_opt -C no-prepopulate-passes -Z verbose -Z no-landing-pads --target=i686-unknown-linux-gnu -o .\rmain.o --emit=obj -Ctarget-feature=+soft-float -C relocation-model=dynamic-no-pic

$list = New-Object `System.Collections.Generic.List[string]`;

# modules
$list.Add("modules/protected/out.s");
$list.Add("modules/protected/vga.c");
$list.Add("modules/protected/draw.c");
$list.Add("modules/protected/asm_wrap.s");
$list.Add("modules/protected/kernel_panic.c");
$list.Add("modules/protected/interrupt.c");
$list.Add("modules/protected/keyboard.c");
$list.Add("modules/protected/timer.c");
$list.Add("modules/protected/calc_mod.c");
$list.Add("modules/protected/memory_ope.c");
$list.Add("modules/protected/interrupt_task.c");
$list.Add("modules/protected/vector.c");

$index = 0;
$cmd = "wsl i686-unknown-linux-gnu-ld -T settings/linker.ld -o cmain.bin rmain.o cmain.o ";
foreach($e in $list) {
  $comp_obj = "wsl gcc $e -c -g -m32 -o obj_$index.o -O" + $c_opt.ToString() + " -nostdlib -nostdinc -fno-builtin"
  Invoke-Expression $comp_obj;
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


