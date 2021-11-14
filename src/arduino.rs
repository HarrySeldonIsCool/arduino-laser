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

pub fn get_data() -> Result<(f64, f64, u32), std::io::Error>{
    //initialise new serial port and establish connection
    let mut port = serialport::new("/dev/ttyACM0", 115200).open()?;
    //initialise buffer
    let mut buffy = [0; 4];
    let mut it_x = vec![];
    let mut it_y = vec![];

    //wait for some signal, that means connection is ready
    send_n_get(&mut port, None, &mut buffy)?;
    let pwr = u32::from_le_bytes(buffy);
    send_n_get(&mut port, Some(&[b'm']), &mut buffy)?;
    let r_whole = u32::from_le_bytes(buffy);
    send_n_get(&mut port, Some(&[b'x']), &mut buffy)?;
    it_x.push(u32::from_le_bytes(buffy));
    //repeatedly send command 'w' and print the response as integer
    loop{
        send_n_get(&mut port, None, &mut buffy)?;
        let x = u32::from_le_bytes(buffy);
        if x == 10000{
            break;
        }
        it_x.push(x);
    }

    send_n_get(&mut port, Some(&[b'y']), &mut buffy)?;
    it_y.push(u32::from_le_bytes(buffy));
    //repeatedly send command 'w' and print the response as integer
    loop{
        send_n_get(&mut port, None, &mut buffy)?;
        let y = u32::from_le_bytes(buffy);
        if y == 10000{
            break;
        }
        it_y.push(y);
    }

    let rx = process(&it_x, pwr);

    let ry = process(&it_y, pwr);

    Ok((rx*0.04, ry*0.04, r_whole))
}

fn process(what: &Vec<u32>, pwr: u32) -> f64{
    ((
        what
        .iter()
        .enumerate()
        .map(|(i, x)| ((i as f64)-((what.len()/2) as f64)).powi(2)*(*x as f64))
        .sum::<f64>()
    )/(pwr as f64)).sqrt()
}