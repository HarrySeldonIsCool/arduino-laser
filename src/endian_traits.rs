pub trait FromEndian{
    fn from_le_bytes(a: &[u8]) -> Self;
    fn from_be_bytes(a: &[u8]) -> Self;
}

macro_rules! from_end {
    ($($target:ty),*) => {$(
        impl FromEndian for $target{
            fn from_le_bytes(a: &[u8]) -> $target{
                let b: [u8; std::mem::size_of::<$target>()] = a.try_into().unwrap();
                <$target>::from_le_bytes(b)
            }

            fn from_be_bytes(a: &[u8]) -> $target{
                let b: [u8; std::mem::size_of::<$target>()] = a.try_into().unwrap();
                <$target>::from_be_bytes(b)
            }
        }
    )*};
}

from_end!(u8, u16, u32, u64, u128, i8, i16, i32, i64, i128, f32, f64);

pub trait ToEndian{
    fn to_le_byte_vec(self) -> Vec<u8>;
    fn to_be_byte_vec(self) -> Vec<u8>;
}

macro_rules! to_end {
    ($($target:ty),*) => {$(
        impl ToEndian for $target{
            fn to_le_byte_vec(self) -> Vec<u8>{
                self.to_le_bytes().to_vec()
            }

            fn to_be_byte_vec(self) -> Vec<u8>{
                self.to_le_bytes().to_vec()
            }
        }
    )*};
}

to_end!(u8, u16, u32, u64, u128, i8, i16, i32, i64, i128, f32, f64);