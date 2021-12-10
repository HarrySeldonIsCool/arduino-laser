use std::ops::*;

#[derive(Clone, Debug)]
pub struct Vector(pub Vec<f32>);

impl Add for &Vector{
    type Output = Vector;

    fn add(self, rhs: Self) -> Self::Output{
        Vector(self
            .0
            .iter()
            .zip(rhs.0.iter())
            .map(|(a, b)| a+b)
            .collect()
        )
    }
}

impl Add<f32> for &Vector{
    type Output = Vector;

    fn add(self, rhs: f32) -> Self::Output{
        Vector(self
            .0
            .iter()
            .map(|a| a+rhs)
            .collect()
        )
    }
}

impl AddAssign<f32> for Vector{
    fn add_assign(&mut self, rhs: f32){
        *self = &self.clone() + rhs;
    }
}

impl Sub<f32> for &Vector{
    type Output = Vector;

    fn sub(self, rhs: f32) -> Self::Output{
        Vector(self
            .0
            .iter()
            .map(|a| a-rhs)
            .collect()
        )
    }
}

impl SubAssign<f32> for Vector{
    fn sub_assign(&mut self, rhs: f32){
        *self = &self.clone() - rhs;
    }
}

impl Mul<f32> for &Vector{
    type Output = Vector;

    fn mul(self, rhs: f32) -> Self::Output{
        Vector(self
            .0
            .iter()
            .map(|a| a*rhs)
            .collect()
        )
    }
}

impl MulAssign<f32> for Vector{
    fn mul_assign(&mut self, rhs: f32){
        *self = &self.clone()*rhs;
    }
}

impl Div<f32> for &Vector{
    type Output = Vector;

    fn div(self, rhs: f32) -> Self::Output{
        Vector(self
            .0
            .iter()
            .map(|a| a/rhs)
            .collect()
        )
    }
}

impl DivAssign<f32> for Vector{
    fn div_assign(&mut self, rhs: f32){
        *self = &self.clone()*rhs;
    }
}

impl Sub for &Vector{
    type Output = Vector;

    fn sub(self, rhs: Self) -> Self::Output{
        Vector(self
            .0
            .iter()
            .zip(rhs.0.iter())
            .map(|(a, b)| a-b)
            .collect()
        )
    }
}

impl SubAssign for Vector{
    fn sub_assign(&mut self, rhs: Vector){
        *self = &self.clone() - &rhs;
    }
}

impl AddAssign for Vector{
    fn add_assign(&mut self, rhs: Vector){
        *self = &self.clone() + &rhs;
    }
}

impl<'a> std::iter::Sum<&'a Vector> for Vector{
    fn sum<I>(iter: I) -> Self
    where I: Iterator<Item = &'a Vector>{
        iter.fold(Vector(vec![0.0; 80]), |x, y| &x+y)
    }
}

pub fn nedler_mead(f: &[Box<dyn Fn(Vec<f32>) -> f32>], first_est: &[f32]) -> Vec<f32>{
    let now = std::time::SystemTime::now();
    let mut simplex = vec![Vector(first_est.to_vec()), Vector(first_est.iter().map(|x| x+0.1).collect()), Vector(first_est.iter().map(|x| x*1.1).collect()), Vector(first_est.iter().enumerate().map(|(i, x)| x+0.2*(i % 2) as f32-0.1).collect())];
    let real_f = |a: Vec<f32>| f.iter().map(|x| x(a.clone())*x(a.clone())).sum::<f32>();
    for _i in 0..3000{
        simplex.sort_unstable_by(|a, b| real_f(a.0.clone()).partial_cmp(&real_f(b.0.clone())).unwrap());
        let x_0 = &simplex[..3].iter().sum::<Vector>()/3.0;
        let x_r = &(&x_0*2.0)-&simplex[3];
        if real_f(simplex.iter().min_by(|a, b| real_f(a.0.clone()).partial_cmp(&real_f(b.0.clone())).unwrap()).unwrap().0.clone()) < 0.01{
            break;
        }
        if real_f(simplex[0].0.clone()) > real_f(x_r.0.clone()){
            let x_e = &(&x_r*2.0)-&x_0;
            if real_f(x_r.0.clone()) > real_f(x_e.0.clone()){
                simplex[3] = x_e;
            }
            else{
                simplex[3] = x_r;
            }
        }
        else if real_f(simplex[2].0.clone()) < real_f(x_r.0.clone()){
            let x_c = &(&x_0+&simplex[3])/2.0;
            if real_f(x_c.0.clone()) < real_f(simplex[3].0.clone()){
                simplex[3] = x_c;
            }
            else{
                let x_1 = simplex[0].clone();
                simplex[1..].iter_mut().for_each(|x| *x = &(&*x-&x_1)/2.0);
            }
        }
        else{
            simplex[3] = x_r;
        }
    }
    eprintln!("{}", std::time::SystemTime::now().duration_since(now).unwrap().as_secs_f64());
    let res = simplex.iter().min_by(|a, b| real_f(a.0.clone()).partial_cmp(&real_f(b.0.clone())).unwrap()).unwrap().0.clone();
    eprintln!("{}", real_f(res.clone()));
    res
}