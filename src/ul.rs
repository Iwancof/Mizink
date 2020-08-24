#![no_std]
#![feature(lang_items)]
#![feature(start)]
#![allow(unused)]
#![no_main]
#![feature(rustc_private)]
#![feature(asm)]
#![allow(non_upper_case_globals)]
#![feature(const_mut_refs)]

extern crate libc;
use libc::{ c_void, c_int };
use core::panic::PanicInfo;

extern "C" {
    fn rk_draw_str(x : u32, y : u32, c : u16, s : *const u8) -> ();
    fn rk_draw_char(x : u32, y : u32, col : u16, c : u8) -> ();
    fn ts_draw_num_16(_ : usize) -> ();
    fn malloc(size : u32) -> *mut c_void;
    fn free(p : *mut c_void) -> ();
    fn kernel_panic() -> !;
}

#[lang = "eh_personality"]
extern fn eh_personality() {}

#[no_mangle]
#[start]
unsafe fn rust_entry() -> i32 {
    rk_draw_str(10, 10, 0x010F, b"Hello from Rust".as_ptr());
    9
}

#[panic_handler]
extern fn panic_handler(info : &PanicInfo<'_>) -> ! {
    unsafe {
        kernel_panic();
    }
}
