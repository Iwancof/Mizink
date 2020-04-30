#![no_std]
#![feature(lang_items)]
#![feature(start)]
#![allow(unused)]
#![no_main]


use core::panic::PanicInfo;
extern "C" {
    fn rk_draw_str(x : u32, y : u32, c : u16, s : *const u8) -> ();
    fn kernel_panic() -> !;
}

#[no_mangle]
#[start]
unsafe fn rust_entry() -> i32 {
    let x = 1;
    10 / x
}

#[lang = "eh_personality"]
extern fn eh_personality() {}

#[panic_handler]
extern fn panic_handler(info : &PanicInfo<'_>) -> ! {
    unsafe {
        kernel_panic();
    }
}

