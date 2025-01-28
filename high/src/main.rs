use std::{thread, time};

mod nextcloud_handler;
mod serial_handler;
mod coordinator;

use crate::coordinator::Coordinator;

fn main() {
    let mut coordinator = Coordinator::new();

    loop {
        coordinator.update();
        thread::sleep(time::Duration::from_secs(30));
    }
}
