use serialport::SerialPort;
use super::process::*;
use super::endian_traits::{Serial, FromEndian, ToEndian};
use libm::erfcf as erfc;
use poloto::plot;
use serial_derive::Serial;
use serde_derive::{Serialize, Deserialize};

enum StopOrTimes<T>{
    Stop(T),
    Times(u64)
}

use StopOrTimes::*;

#[derive(Debug, Copy, Clone, Serial, PartialEq, Serialize, Deserialize, Default)]
pub struct TimenPlaceStamp{
    pub time: u32,
    pub place: i64,
    pub a: f32
}

fn wait_for_answer(port: &mut Box<dyn SerialPort>) -> Result<(), std::io::Error>{
    while port.bytes_to_read()? == 0{}
    Ok(())
}

fn send_n_get(port: &mut Box<dyn SerialPort>, sent: Option<&[u8]>, buffer: &mut [u8]) -> Result<(), std::io::Error>{
    port.clear(serialport::ClearBuffer::Input)?;
    if let Some(x) = sent{
        port.write(x)?;
    }
    wait_for_answer(port)?;
    port.read_exact(buffer)
}

fn send_n_get_n<T, U>(port: &mut Box<dyn SerialPort>, sent: Option<U>) -> Result<T, std::io::Error>
where T: Serial + Default, U: Serial{
    let mut buffer = vec![0;2];
    let mut help;
    let real_sent = match sent{
        Some(x) => {help = x.ser(); help = [(help.len() as u16).ser(), help].concat(); Some(&help[..])},
        None => None
    };
    send_n_get(port, real_sent, &mut buffer)?;
    let mut buffer1 = vec![0;u16::from_le_byte_slice(&buffer) as usize];
    port.read_exact(&mut buffer1)?;
    Ok(T::deser(&buffer1))
}

pub fn test_struct() -> Result<(), std::io::Error>{
    let mut port = serialport::new("/dev/ttyACM3", 115200).timeout(std::time::Duration::from_secs(1)).open()?;
    wait_for_answer(&mut port)?;
    loop{
    let a: TimenPlaceStamp = send_n_get_n::<TimenPlaceStamp, u8>(&mut port, None)?;
    //let a: i32 = i32::from_le_bytes(buffer);
    println!("{:?}", a);
    //println!("{}", std::mem::size_of::<Rec>());
    }
    Ok(())
}

fn send_n_get_vec<T, U>(port: &mut Box<dyn SerialPort>, sent: Option<U>, stop_or_times: StopOrTimes<T>) -> Result<Vec<T>, std::io::Error>
where T: Serial + PartialEq + Copy + Default, U: Serial + PartialEq + Copy{
    let mut x = send_n_get_n(port, sent)?;
    let mut it_x = vec![x];
    match stop_or_times{
        Stop(stop) => {while x != stop{
            x = send_n_get_n(port, Some(1u8))?;
            it_x.push(x);
        }
        it_x.pop();},
        Times(times) => for _i in 1..times{
            x = send_n_get_n(port, Some(1u8))?;
            it_x.push(x);
        }
    }
    Ok(it_x)
}

pub fn get_raw(n: u32, dt: u32, dx: u32) -> Result<(Vec<TimenPlaceStamp>, f32, f32, f32, f32), std::io::Error>{
    let mut port = serialport::new("/dev/ttyACM0", 115200).timeout(std::time::Duration::from_secs(1)).open()?;
    wait_for_answer(&mut port)?;
    port.write(&n.to_le_bytes())?;
    std::thread::sleep(std::time::Duration::from_millis(200));
    port.write(&dt.to_le_bytes())?;
    std::thread::sleep(std::time::Duration::from_millis(200));

    let a_0 = send_n_get_n(&mut port, Some(dx))?;
    let n_0 = send_n_get_n::<f32, u8>(&mut port, None)?;
    let s_0 = send_n_get_n::<f32, u8>(&mut port, None)?;

    let it_x = send_n_get_vec::<TimenPlaceStamp, u8>(&mut port, None, Stop(TimenPlaceStamp{time: 0, place: 0, a: 10000.0}))?;

    Ok((it_x[1..].to_vec(), it_x[0].a, a_0, n_0, s_0))
}

pub fn get_res(n: u32, dt: u32, dx: u32) -> Result<f32, std::io::Error>{
    let (it_x, pwr, a_0, n_0, s_0) = get_raw(n, dt, dx)?;

    let w = it_x.len() - 2*it_x
        .iter()
        .enumerate()
        .map(|(i, x)| (i, (x.a - pwr/2f32.exp()).abs()))
        .min_by(|(_, x),(_, y)| x.partial_cmp(y).unwrap()).unwrap().0;
    

    let fit = process(&it_x, pwr, (w as f32)/2.0, a_0, n_0, s_0);

    plot("Intensity", "x", "I(x)")
        .line("int", it_x.iter().enumerate().map(|(i, x)| (i, x.a)))
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

    Ok(fit[2])
}