#[repr(C)]
pub struct EventArgs {
    pub v : i32,
}

#[repr(C)]
struct TaskUnit {
    t_id : i32,
    event_fp : extern fn(ar : EventArgs, *const u8) -> i32,
}

#[repr(C)]
pub struct TaskPool {
  tasks : *mut TaskUnit,
}

macro_rules! int {
    ($pool:expr, $fp:expr, $proc:block) => {
        let __interrupt_handler = subscribe(&mut $pool as *mut TaskPool, $fp);
        $proc;
        cancel(&mut $pool as *mut TaskPool, __interrupt_handler);
    }
}
