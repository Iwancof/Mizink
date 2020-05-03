#![no_std]
#![feature(lang_items)]
#![feature(start)]
#![allow(unused)]
#![no_main]
#![feature(rustc_private)]
#[warn(non_upper_case_globals)]

extern crate libc;

use libc::{ c_void, c_int };
use core::panic::PanicInfo;
use core::mem::{ size_of };
use core::marker::PhantomData;
use core::clone::Clone;
use core::ops::{ Index, IndexMut };

#[repr(C)]
#[derive(Copy, Clone)]
struct rk_vec {
    next : *mut rk_vec,
    element : *mut c_void,
}

extern "C" {
    fn rk_draw_str(x : u32, y : u32, c : u16, s : *const u8) -> ();
    fn ts_draw_num_16(_ : usize) -> ();
    fn malloc(size : u32) -> *mut c_void;
    fn free(p : *mut c_void) -> ();
    fn kernel_panic() -> !;
    fn list_new() -> *mut rk_vec;
    fn list_add(v : *mut *mut rk_vec,e : *const c_void, size : usize) -> ();
    fn get_vec_at(v : *const rk_vec, index : usize) -> *mut rk_vec;
    fn get_at(v : *const rk_vec, index : usize) -> *mut c_void;
    fn list_erase(v : *mut *mut rk_vec, index : usize) -> *mut c_void;
    fn list_copy(v : *mut rk_vec, size : usize) -> *mut rk_vec;
    fn list_free(v : *mut rk_vec) -> ();
    fn ts_show_all(v : *const rk_vec) -> ();
    static mut current_task_name : *const u8;
}

struct Vec<T : Clone> {
    v : *mut rk_vec,
    phantom : PhantomData<T>,
}
impl<T : Clone> Vec<T> {
    pub fn new() -> Self {
        unsafe {
            Self {
                v : list_new(),
                phantom : PhantomData,
            }
        }
    }
    pub fn push(&mut self, e : T) {
        unsafe {
            list_add(&mut self.v as *mut *mut rk_vec, &e as *const T as *const c_void, size_of::<T>());
        }
    }
    pub fn erase(&mut self, index : usize) -> T {
        unsafe {
            let ptr = list_erase(self.v as *mut *mut rk_vec, index);
            let ret = (*(ptr as *const T)).clone();
            free(ptr);
            ret
        }
    }
    fn get_vec_at(&self, index : usize) -> *mut rk_vec {
        unsafe {
            get_vec_at(self.v as *const rk_vec, index)
        }
    }
    fn get_at(&self, index : usize) -> *mut T {
        unsafe {
            get_at(self.v as *const rk_vec, index) as *mut T
        }
    }
    pub fn ts_show_all(&self) -> () {
        unsafe {
            ts_show_all(self.v);
        }
    }
    pub fn iter(&mut self) -> VecIter<T> {
        VecIter {
            v: self.v,
            phantom : PhantomData,
        }
    }
}
impl<T : Clone> Index<usize> for Vec<T> {
    type Output = T;
    fn index(&self, index : usize) -> &Self::Output {
        unsafe {
            let ptr = self.get_at(index);
            if ptr as i32 == 0 {
                panic!();
            }
            &*ptr
        }
    }
}
impl<T : Clone> IndexMut<usize> for Vec<T> {
    fn index_mut(&mut self, index : usize) -> &mut Self::Output {
        unsafe {
            let ptr = self.get_at(index);
            if ptr as i32 == 0 {
                panic!();
            }
            &mut *ptr
        }
    }
}
impl<T : Clone> Clone for Vec<T> {
    fn clone(&self) -> Self {
        unsafe {
            Self{
                v : list_copy(self.v, size_of::<T>()),
                phantom : PhantomData,
            }
        }
    }
}
impl<T : Clone> Drop for Vec<T> {
    fn drop(&mut self) {
        unsafe {
            list_free(self.v);
        }
    }
}
struct VecIter<T : Clone> {
    v : *mut rk_vec,
    phantom : PhantomData<T>,
}
impl<T : Clone> Iterator for VecIter<T> {
    type Item = T;
    fn next(&mut self) -> Option<T> {
        if self.v as usize == 0 {
            return None;
        } else {
            unsafe {
                let vec_elm = *self.v;
                self.v = vec_elm.next;
                return Some((*(vec_elm.element as *mut T)).clone());
            }
        }
    }
}

static rust_task_name : &str = "RUST:KEYBOARD";
static mut is_end : bool = true;

#[no_mangle]
#[start]
unsafe fn rust_entry() -> i32 {
    let current_task_name_tmp = current_task_name;
    current_task_name = rust_task_name.as_ptr();

    let mut v = Vec::<i32>::new();
    v.push(2);
    for i in 2..100 {
        if !v.iter().any(|x| i % x == 0) || i == 57 {
            v.push(i);
        }
    }
    v.ts_show_all();

    
    while(unsafe { is_end });

    //ts_draw_num_16(size_of::<i32>());

    current_task_name = current_task_name_tmp;
    0
}
fn ts_end() {
    unsafe {
        is_end = false;
    }
}

#[lang = "eh_personality"]
extern fn eh_personality() {}

#[panic_handler]
extern fn panic_handler(info : &PanicInfo<'_>) -> ! {
    unsafe {
        kernel_panic();
    }
}

