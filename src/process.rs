use super::math::*;
use super::arduino::TimeStamp;
use libm::{erfcf as erfc, sinf as sin};

pub fn process(what: &Vec<TimeStamp>, pwr: f32, w: f32, a_0: f32, n_0: f32, s_0: f32) -> Vec<f32>{

    let f = what
        .iter()
        .enumerate()
        .map(|(x, &i)| Box::new(move|a: Vec<f32>| {
            let p = a[0];
            let x_0 = a[1];
            let w = a[2];
            let b = a[3];
	        let neco = a[4];
	        let n = a[5];
	        let s = a[6];
            i.a-p/2.0*erfc((x as f32 - x_0)/w)-b-neco*sin(n*(i.time as f32+s))
        }) as Box<dyn Fn(Vec<f32>) -> f32>)
        .collect::<Vec<Box<dyn Fn(Vec<f32>) -> f32>>>();

    nedler_mead(
        &f[..],
        &[
            pwr,
            (what.len() as f32)/2.0,
            w,
            0.005,
	        a_0,
	        n_0,
	        s_0
        ],
    )
}