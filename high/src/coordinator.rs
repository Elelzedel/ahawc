use std::{thread, time};

use crate::nextcloud_handler::NextcloudHandler;
use crate::serial_handler::SerialHandler;

pub struct Coordinator {
    serial_handler: SerialHandler,
    nextcloud_handler: NextcloudHandler,
    well_to_use: i8,
}

impl Coordinator {
    const WELLS_COUNT: u8 = 3;
    const TRIGGER_LEVEL: i8 = 28;
    const MIN_RISE_RATES: [u8; 3] = [1, 1, 1];
    const CUTOFF_CURRENTS: [f32; 3] = [4.5, 7.5, 7.0];

    pub fn new() -> Coordinator {
        let serial_handler = SerialHandler::new();
        let nextcloud_handler = NextcloudHandler::new();

        nextcloud_handler.send_message("AHAWC online");

        return Coordinator {
            serial_handler,
            nextcloud_handler,
            well_to_use: 0,
        };
    }

    pub fn update(&mut self) {
        self.handle_messages();

        let active = self.serial_handler.get_active();
        let level = self.serial_handler.get_level();

        if active < 0 && level < Self::TRIGGER_LEVEL && level > 0 {
            self.set_active();
            self.well_to_use += 1;

            if self.well_to_use >= Self::WELLS_COUNT.try_into().unwrap() {
                self.well_to_use = 0;
            }
        }
    }

    fn set_active(&self) {
        let well_to_use_usize: usize = self.well_to_use.try_into().unwrap();
        loop {
            let successful = self.serial_handler.set_active_well(
                self.well_to_use,
                Self::MIN_RISE_RATES[well_to_use_usize],
                Self::CUTOFF_CURRENTS[well_to_use_usize],
            );

            println!("{}", successful);

            if successful {
                return;
            }
            println!("Retry");

            self.serial_handler.set_active_well(-1, 0, 0.0);
            println!("Sleeping");
            thread::sleep(time::Duration::from_secs(5));
        }
    }

    fn handle_messages(&self) {
        let messages = self.serial_handler.get_message_queue();

        if messages.iter().any(|e| e.contains("PC DISABLED TANK PUMP")) {
            self.nextcloud_handler.send_message("Disabled tank pump.");
        }

        if messages
            .iter()
            .any(|e| e.contains("PC REENABLED TANK PUMP"))
        {
            self.nextcloud_handler.send_message("Re-enabled tank pump.");
        }
    }
}
