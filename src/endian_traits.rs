pub trait FromEndian{
    fn from_le_byte_slice(a: &[u8]) -> Self;
    fn from_be_byte_slice(a: &[u8]) -> Self;
}

macro_rules! from_end {
    ($($target:ty),*) => {$(
        impl FromEndian for $target{
            fn from_le_byte_slice(a: &[u8]) -> $target{
                let b: [u8; std::mem::size_of::<$target>()] = a.try_into().unwrap();
                <$target>::from_le_bytes(b)
            }

            fn from_be_byte_slice(a: &[u8]) -> $target{
                let b: [u8; std::mem::size_of::<$target>()] = a.try_into().unwrap();
                <$target>::from_be_bytes(b)
            }
        }
    )*};
}

from_end!(u8, u16, u32, u64, u128, i8, i16, i32, i64, i128, f32, f64, usize);

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

to_end!(u8, u16, u32, u64, u128, i8, i16, i32, i64, i128, f32, f64, usize);

pub trait Serial{
    fn deser(buffer: &[u8]) -> Self;
    fn ser(&self) -> Vec<u8>;
}

macro_rules! serial_imp {
    ($($target:ty),*) => {$(impl Serial for $target {
        fn deser(buffer: &[u8]) -> Self{
            rmp_serde::from_read(buffer).unwrap()
        }
        fn ser(&self) -> ::std::vec::Vec<u8>{
            rmp_serde::to_vec(self).unwrap()
        }
    }
    )*};
}

serial_imp!(u8, u16, u32, u64, u128, i8, i16, i32, i64, i128, f32, f64, usize, Vec<u8>);