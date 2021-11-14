use serialport::SerialPort;

//wait until there's some data to read
fn wait_for_answer(port: &mut Box<dyn SerialPort>) -> Result<(), std::io::Error>{
    while port.bytes_to_read()? == 0{
        std::thread::sleep(std::time::Duration::from_millis(50));
    }
    Ok(())
}

//send array of bytes, then wait for response and put it into buffer (as much as can fit)
fn send_n_get(port: &mut Box<dyn SerialPort>, sent: &[u8], buffer: &mut [u8]) -> Result<(), std::io::Error>{
    port.clear(serialport::ClearBuffer::Input)?;
    port.write(sent)?;
    wait_for_answer(port)?;
    port.read(buffer)?;
    Ok(())
}

fn main() -> Result<(), std::io::Error>{
    //initialise new serial port and establish connection
    let mut port = serialport::new("/dev/ttyACM0", 115200).open()?;
    //initialise buffer
    let mut buffy = [0; 4];

    //wait for some signal, that means connection is ready
    wait_for_answer(&mut port)?;
    //clear input buffer so that it won't get messy
    port.clear(serialport::ClearBuffer::Input)?;

    //repeatedly send command 'w' and print the response as integer
    loop{
        send_n_get(&mut port, &[b'w'], &mut buffy)?;
        let x = i32::from_le_bytes(buffy);
        eprint!("\rValue is: {}", x);
    }

    //Ok(())
}