use serialport::SerialPort;

//wait until there's some data to read
fn wait_for_answer(port: &mut Box<dyn SerialPort>) -> Result<(), std::io::Error>{
    while port.bytes_to_read()? == 0{
        std::thread::sleep(std::time::Duration::from_millis(50));
    }
    Ok(())
}

//send optional array of bytes, then wait for response and put it into buffer (as much as can fit)
fn send_n_get(port: &mut Box<dyn SerialPort>, sent: Option<&[u8]>, buffer: &mut [u8]) -> Result<(), std::io::Error>{
    port.clear(serialport::ClearBuffer::Input)?;
    if let Some(x) = sent{
        port.write(x)?;
    }
    wait_for_answer(port)?;
    port.read(buffer)?;
    Ok(())
}

pub fn get_data() -> Result<f64, std::io::Error>{
    //initialise new serial port and establish connection
    let mut port = serialport::new("/dev/ttyACM0", 115200).open()?;
    //initialise buffer
    let mut buffy = [0; 4];
    let mut it_be = vec![];

    //wait for some signal, that means connection is ready
    wait_for_answer(&mut port)?;
    //clear input buffer so that it won't get messy
    port.clear(serialport::ClearBuffer::Input)?;

    //repeatedly send command 'w' and print the response as integer
    loop{
        send_n_get(&mut port, None, &mut buffy)?;
        let x = u32::from_le_bytes(buffy);
        if x == 10000{
            break;
        }
        it_be.push(x);
    }

    let lenght = it_be.len();
    let it_is = it_be[..(lenght/2+{if lenght%2 == 0{0}else{1}})].to_vec();

    let r = process(&it_is);

    Ok(r)
}

fn process(what: &Vec<u32>) -> f64{
    0.0
}