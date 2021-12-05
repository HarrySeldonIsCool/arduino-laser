mod arduino;
mod math;
mod endian_traits;
mod process;
use self::arduino::*;

fn main() -> Result<(), std::io::Error>{
    //rgsl::types::MultiFitFdfSolver::new(&rgsl::types::MultiFitFdfSolverType::lmder(),43,5);
    println!("{}", get_res(100, 200, 10)?);
    Ok(())
}

#[test]
fn test1() -> Result<(), std::io::Error>{
    use poloto::plot;
    use libm::erfcf as erfc;
    let data = vec![2.0,
    1.99,
    1.97,
    1.96,
    1.95,
    1.93,
    1.92,
    1.9,
    1.87,
    1.82,
    1.75,
    1.7,
    1.55,
    1.46,
    1.37,
    1.27,
    1.17,
    1.06,
    0.944,
    0.84,
    0.742,
    0.64,
    0.536,
    0.451,
    0.372,
    0.296,
    0.233,
    0.183,
    0.142,
    0.108,
    0.08,
    0.059,
    0.043,
    ];
    let fit = self::process::process(&data, 2.0, 7.0);
    println!("{:?}", fit);
    plot("Intensity", "x", "I(x)")
        .line("int", data.iter().enumerate())
        .line("fit", (0..data.len()).map(|x| (x, {
            let p = fit[0];
            let x_0 = fit[1];
            let w = fit[2];
            let b = fit[3];
            p/2.0*erfc((x as f32 - x_0)/w)+b
        })))
        .xmarker(0)
        .ymarker(0)
        .simple_theme(poloto::upgrade_write(std::fs::File::create(std::path::Path::new("/home/davidek/projects-rust/arduino-laser/src/graphs/graph1.svg"))?));
    Ok(())
}