mod arduino;
mod math;
mod endian_traits;
mod process;
use self::arduino::*;

fn main() -> Result<(), std::io::Error>{
    //println!("{}", get_res(&std::env::args().last().unwrap(), 100, 200, 10)?);
    test_struct(&std::env::args().last().unwrap());
    Ok(())
}