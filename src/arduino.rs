use serialport::SerialPort;
use super::process::*;
use super::endian_traits::FromEndian;
use libm::erfcf as erfc;
use poloto::plot;

enum StopOrTimes<T>{
    Stop(T),
    Times(u64)
}

use StopOrTimes::*;

pub fn wait_for_answer(port: &mut Box<dyn SerialPort>) -> Result<(), std::io::Error>{
    while port.bytes_to_read()? == 0{}
    Ok(())
}

fn send_n_get(port: &mut Box<dyn SerialPort>, sent: Option<&[u8]>, buffer: &mut [u8]) -> Result<(), std::io::Error>{
    port.clear(serialport::ClearBuffer::Input)?;
    if let Some(x) = sent{
        port.write(x)?;
    }
    wait_for_answer(port)?;
    port.read(buffer)?;
    Ok(())
}

fn send_n_get_n<T>(port: &mut Box<dyn SerialPort>, sent: Option<&[u8]>) -> Result<T, std::io::Error>
where T: FromEndian{
    let mut buffer = vec![0;std::mem::size_of::<T>()];
    send_n_get(port, sent, &mut buffer[..])?;
    Ok(T::from_le_bytes(&buffer[..]))
}

fn send_n_get_vec<T>(port: &mut Box<dyn SerialPort>, sent: Option<&[u8]>, stop_or_times: StopOrTimes<T>) -> Result<Vec<T>, std::io::Error>
where T: FromEndian + PartialEq + Copy{
    let mut x = send_n_get_n::<T>(port, sent)?;
    let mut it_x = vec![x];
    match stop_or_times{
        Stop(stop) => {while x != stop{
            x = send_n_get_n(port, Some(&[b'a']))?;
            it_x.push(x);
        }
        it_x.pop();},
        Times(times) => for _i in 1..times{
            x = send_n_get_n(port, Some(&[b'a']))?;
            it_x.push(x);
        }
    }
    Ok(it_x)
}

pub fn get_raw(n: u32, dt: u32, dx: u32) -> Result<(Vec<f32>, f32), std::io::Error>{
    let mut port = serialport::new("/dev/ttyACM0", 115200).open()?;
    wait_for_answer(&mut port)?;
    port.write(&n.to_le_bytes())?;
    port.write(&dt.to_le_bytes())?;

    let it_x = send_n_get_vec(&mut port, Some(&dx.to_le_bytes()), Stop(10000f32))?;

    Ok((it_x[1..].to_vec(), it_x[0]))
}

pub fn get_res(n: u32, dt: u32, dx: u32) -> Result<f32, std::io::Error>{
    let (it_x, pwr) = get_raw(n, dt, dx)?;

    let w = it_x.len() - 2*it_x
        .iter()
        .enumerate()
        .map(|(i, x)| (i, (x - pwr/2f32.exp()).abs()))
        .min_by(|(_, x),(_, y)| x.partial_cmp(y).unwrap()).unwrap().0;
    

    let fit = process(&it_x, pwr, (w as f32)/2.0);

    plot("Intensity", "x", "I(x)")
        .line("int", it_x.iter().enumerate())
        .line("fit", (0..it_x.len()).map(|x| (x, {
            let p = fit[0];
            let x_0 = fit[1];
            let w = fit[2];
            let b = fit[3];
            p/2.0*erfc((x as f32 - x_0)/w)+b
        })))
        .xmarker(0)
        .ymarker(0)
        .simple_theme(poloto::upgrade_write(std::fs::File::create(std::path::Path::new("/home/davidek/projects-rust/arduino-laser/src/graphs/graph.svg"))?));

    let rx = fit[2];

    Ok(rx)
}