use super::math::*;
use libm::erfcf as erfc;

pub fn process(what: &Vec<f32>, pwr: f32, w: f32) -> Vec<f32>{

    let f = what
        .iter()
        .enumerate()
        .map(|(x, &i)| Box::new(move|a: Vec<f32>| {
            let p = a[0];
            let x_0 = a[1];
            let w = a[2];
            let b = a[3];
            i-p/2.0*erfc((x as f32 - x_0)/w)-b
        }) as Box<dyn Fn(Vec<f32>) -> f32>)
        .collect::<Vec<Box<dyn Fn(Vec<f32>) -> f32>>>();

    nedler_mead(
        &f[..],
        &[
            pwr,
            (what.len() as f32)/2.0,
            w,
            0.005
        ],
    )
}