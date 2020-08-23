use core::marker::PhantomData;
use core::ops::{ Index, IndexMut };
use core::mem::{ size_of };
use core::clone::Clone;
use core::fmt::{ Write, Error, Arguments };
use libc::{ c_void, c_int };

use {
    rk_draw_char,
    list_new,
    list_add,
    get_vec_at,
    get_at,
    list_erase,
    list_copy,
    list_free,
    ts_show_all,
    free,
};

#[repr(C)]
#[derive(Copy, Clone)]
pub struct rk_vec {
    pub next : *mut rk_vec,
    pub element : *mut c_void,
}
pub static mut null_rk : rk_vec = rk_vec {
    next : 0 as *mut rk_vec,
    element : 0 as *mut c_void,
};
pub struct Vec<T : Clone> {
    pub v : *mut rk_vec,
    pub phantom : PhantomData<T>,
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
    pub fn iter(&self) -> VecIter<T> {
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
pub struct VecIter<T : Clone> {
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

#[derive(Clone)]
pub struct String {
    s : Vec<u8>,
}

impl Write for String {
    fn write_str(&mut self,cs : &str) -> Result<(), Error> {
        for c in cs.chars() {
            self.push_char(c as u8);
        }
        Ok(())
    }
}
impl String {
    fn new() -> String {
        String { s : Vec::new() }
    }
    fn push_char(&mut self, c : u8) {
        self.s.push(c);
    }
    pub fn print_at(&self, mut x : u32, mut y : u32) {
        for c in self.s.iter() {
            unsafe {
                rk_draw_char(x, y, 0x010F, c);
            }
            x += 1;
        }
    }
}

pub fn format(args: Arguments<'_>) -> String {
    let mut output = String::new();
    output
        .write_fmt(args)
        .expect("a formatting trait implementation returned an error");
    output
}

macro_rules! format {
    ($($arg:tt)*) => {{
        let res = format(format_args!($($arg)*));
        res
    }}
}
