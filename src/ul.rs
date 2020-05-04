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
#[macro_use]
pub mod modules {
    #[macro_use]
    pub mod rust_mods {
        #[macro_use]
        pub mod vec;
        #[macro_use]
        pub mod interrupt;
    }
}
use modules::rust_mods::vec::{
    rk_vec,
    Vec,
    VecIter,
    format,
};
use modules::rust_mods::interrupt::{
    EventArgs,
    TaskPool,
};

use libc::{ c_void, c_int };
use core::panic::PanicInfo;
use core::ptr::{ read_volatile, write_volatile };

extern "C" {
    fn rk_draw_str(x : u32, y : u32, c : u16, s : *const u8) -> ();
    fn rk_draw_char(x : u32, y : u32, col : u16, c : u8) -> ();
    fn ts_draw_num_16(_ : usize) -> ();
    fn malloc(size : u32) -> *mut c_void;
    fn free(p : *mut c_void) -> ();
    fn kernel_panic() -> !;
    pub fn list_new() -> *mut rk_vec;
    pub fn list_add(v : *mut *mut rk_vec,e : *const c_void, size : usize) -> ();
    pub fn get_vec_at(v : *const rk_vec, index : usize) -> *mut rk_vec;
    pub fn get_at(v : *const rk_vec, index : usize) -> *mut c_void;
    fn list_erase(v : *mut *mut rk_vec, index : usize) -> *mut c_void;
    pub fn list_copy(v : *mut rk_vec, size : usize) -> *mut rk_vec;
    pub fn list_free(v : *mut rk_vec) -> ();
    fn ts_show_all(v : *const rk_vec) -> ();
    fn subscribe_key_proc(proc : extern fn(ar : EventArgs, task_name : *const u8)) -> i32;
    fn cancel_key_proc(id : i32) -> ();
    fn subscribe_timer_proc(proc : extern fn(ar : EventArgs, task_name : *const u8)) -> i32;
    fn cancel_timer_proc(id : i32) -> ();
    fn subscribe(pool_ptr : *mut  TaskPool, proc : extern fn(ar : EventArgs, sender : *const u8)) -> i32;
    fn cancel(pool_ptr : *mut TaskPool, t_id : i32) -> ();
    static mut current_task_name : *const u8;
    static mut keyboard_task_pool : TaskPool;
    static mut timer_task_pool : TaskPool;
}

static rust_task_name : &str = "RUST:KEYBOARD";
static mut is_end : *mut bool = &mut false as *mut bool;

#[no_mangle]
#[start]
unsafe fn rust_entry() -> i32 {
    let current_task_name_tmp = current_task_name;
    current_task_name = rust_task_name.as_ptr();

    let mut field = Vec::<Vec<i32>>::new();
    let mut list = Vec::<i32>::new();
    for _ in 0..22 {
        list.push(0);
    }
    for _ in 0..12 {
        field.push(list.clone());
    }
    for i in 0..22 {
        field[0][i] = 1;
        field[11][i] = 1;
    }
    for i in 0..12 {
        field[i][0] = 1;
        field[i][21] = 1;
    }

    int!(keyboard_task_pool, tet_key, {
        int!(timer_task_pool, tet_tim, {
            loop {
                for x in 0..12 {
                    for y in 0..22 {
                        format!("{}", field[x][y]).print_at(x as u32, y as u32);
                    }
                }
            } 
        });
    });

    current_task_name = current_task_name_tmp;
    0
}

static mut c : u32 = 0;
extern fn tet_key(e : EventArgs, s : *const u8) {
    unsafe { rk_draw_str(0, c, 0x010F, b"TEST\0".as_ptr()) };
    unsafe { c += 1; };
}
extern fn tet_tim(e : EventArgs, s : *const u8) {
}

#[lang = "eh_personality"]
extern fn eh_personality() {}

#[panic_handler]
extern fn panic_handler(info : &PanicInfo<'_>) -> ! {
    unsafe {
        kernel_panic();
    }
}

