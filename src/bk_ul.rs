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
    null_rk,
};
use modules::rust_mods::interrupt::{
    EventArgs,
    TaskPool,
};

use libc::{ c_void, c_int };
use core::panic::PanicInfo;
use core::ptr::{ read_volatile, write_volatile };
use core::marker::PhantomData;

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
static mut tet_x : usize = 6;
static mut tet_y : usize = 0;
static mut rot : i32 = 0;
static mut field : Vec<Vec<i32>> = Vec { v : unsafe { &mut null_rk as *mut rk_vec } , phantom : PhantomData };
static mut list : Vec<i32> = Vec { v : unsafe { &mut null_rk as *mut rk_vec } , phantom : PhantomData };
static mut tets : Vec<Vec<Vec<i32>>> = Vec { v : unsafe { &mut null_rk as *mut rk_vec } , phantom : PhantomData };


#[no_mangle]
#[start]
unsafe fn rust_entry() -> i32 {
    asm!(
        "int 10"
        );
    tets = Vec::<Vec<Vec<i32>>>::new();
    let tmp = array_to_vec([
               [0, 0, 0, 0],
               [0, 1, 1, 0],
               [0, 1, 1, 0],
               [0, 0, 0, 0],
    ]);
    /*
    tets.push(array_to_vec([
                [0, 0, 0, 0],
                [0, 1, 1, 0],
                [0, 1, 1, 0],
                [0, 0, 0, 0],
            ]));
            */
    /*
    for x in 0..4 {
        for y in 0..4 {
            format!("{}", tmp[y][x]).print_at(x as u32 * 3, y as u32 * 3);
        }
    }
    */



    return 0;

    let current_task_name_tmp = current_task_name;
    current_task_name = rust_task_name.as_ptr();

    field = Vec::<Vec<i32>>::new();
    list = Vec::<i32>::new();

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
                        if (x,y) == (tet_x, tet_y) {
                            format!("*").print_at(x as u32, y as u32);
                            continue;
                        }
                        match field[x][y] {
                            0 => {
                                format!(" ").print_at(x as u32, y as u32);
                            }
                            1 => {
                                format!("#").print_at(x as u32, y as u32);
                            }
                            2 => {
                                format!("*").print_at(x as u32, y as u32);
                            }
                            _ => {
                                panic!();
                            }
                        }
                    }
                }
            } 
        });
    });

    current_task_name = current_task_name_tmp;
    0
}

unsafe fn tet_down() {
    if field[tet_x][tet_y + 1] != 0 {
        field[tet_x][tet_y] = 2;
        tet_x = 6;
        tet_y = 0;
    } else {
        tet_y += 1;
    }
}

extern fn tet_key(e : EventArgs, s : *const u8) {
    unsafe {
        if e.v == 0x4B { // Left
            if field[tet_x - 1][tet_y] == 0 {
                tet_x -= 1;
            }
        } else if e.v == 0x4D { // Right
            if field[tet_x + 1][tet_y] == 0 {
                tet_x += 1;
            }
        } else if e.v == 57 {
            rot += 1;
        }
    }
}
static mut timer_count : u32 = 0;
extern fn tet_tim(e : EventArgs, s : *const u8) {
    unsafe {
        timer_count += 1;
        timer_count %= 10;
        if timer_count == 0 {
            tet_down();
        }
    }
}
fn array_to_vec(arg : [[i32;4];4]) -> Vec<Vec<i32>> {
    let mut ret = Vec::<Vec<i32>>::new();
    let mut tmp = Vec::<i32>::new();
    for i in 0..4 {
        tmp.push(0);
    }
    for i in 0..4 {
        ret.push(tmp.clone());
    }
    for x in 0..4 {
        for y in 0..4 {
            ret[x][y] = arg[x][y];
        }
    }
    ret.clone()
}


#[lang = "eh_personality"]
extern fn eh_personality() {}

#[panic_handler]
extern fn panic_handler(info : &PanicInfo<'_>) -> ! {
    unsafe {
        kernel_panic();
    }
}

