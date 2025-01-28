use serial2::SerialPort;
use std::{thread, time};

pub struct SerialHandler {
    port: SerialPort,
}

impl SerialHandler {
    pub fn new() -> SerialHandler {
        let port = SerialPort::open("/dev/ttyACM0", 9600).unwrap();
        thread::sleep(time::Duration::from_secs(2));

        return SerialHandler { port };
    }

    pub fn get_active(&self) -> i32 {
        self.port.write(b"WC GET ACTIVE\n").unwrap();
        let active = self.read_line();

        let active = active.split(" ").nth(4).unwrap().replace("\n", "");

        let active = active.trim().parse::<i32>().unwrap();

        return active;
    }

    pub fn get_message_queue(&self) -> Vec<String> {
        self.port.write(b"MQ GET\n").unwrap();

        let mut messages: Vec<String> = Vec::new();

        loop {
            let line = self.read_line();

            if line.starts_with("END") {
                break;
            }

            messages.push(String::from(line.trim()));
        }

        return messages;
    }

    fn read_line(&self) -> String {
        let mut line: Vec<u8> = Vec::new();

        loop {
            let mut buffer = [0; 256];
            let read = self.port.read(&mut buffer);

            if read.is_err() {
                continue;
            }

            let read = read.unwrap();

            line.append(&mut buffer[0..read].to_vec());

            if buffer.contains(&b'\n') {
                break;
            }
        }

        println!("{}", String::from_utf8(line.clone()).unwrap());

        return String::from_utf8(line).unwrap();
    }


    pub fn get_level(&self) -> i8 {
        self.port.write(b"LO GET\n").unwrap();
        let level = self.read_line();

        println!("{}", level);

        let level = level.split(" ").nth(3).unwrap().replace("\n", "");

        let level = level.trim().parse::<i8>().unwrap();

        return level;
    }

    pub fn set_active_well(&self, well: i8, min_rise_rate: u8, cutoff_current: f32) -> bool {
        let command = format!("WC SET ACTIVE {} {} {}", well, min_rise_rate, cutoff_current);

        self.port.write(command.as_bytes()).unwrap();
        let level = self.read_line();

        if level.starts_with("ACK") {
            return true;
        } else {
            return false;
        }
    }
}
