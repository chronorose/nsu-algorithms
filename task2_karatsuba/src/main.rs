use std::{cmp, io, ops::Add};

#[derive(Debug, Clone)]
struct Numbers {
    first: Vec<u8>,
    second: Vec<u8>,
}

fn u8_to_i128(number: &Vec<u8>) -> i128 {
    let mut first = 0;
    for (i, val) in number.into_iter().rev().enumerate() {
        first += 10_i128.pow(i as u32) * ((*val as i128) - 48);
    }
    first
}

fn zero_last_places(vector: &mut Vec<u8>, new_size: usize) -> i32 {
    let mut amount: i32 = 0;
    while vector.len() < new_size {
        vector.push(48);
        amount += 1;
    }
    amount
}

impl Numbers {
    fn make_equal_len(&mut self) -> i32 {
        let mut new_len = cmp::max(self.first.len(), self.second.len());
        if new_len % 2 != 0 {
            new_len += 1;
        }
        let first_diff = zero_last_places(&mut self.first, new_len);
        let second_diff = zero_last_places(&mut self.second, new_len);
        first_diff + second_diff
    }

    fn into_i128(&self) -> (i128, i128) {
        (u8_to_i128(&self.first), u8_to_i128(&self.second))
    }

    fn new(first: i128, second: i128) -> Self {
        let first = first.to_string().into_bytes();
        let second = second.to_string().into_bytes();
        Numbers { first, second }
    }

    fn split_in_two(&mut self) -> Numbers {
        let first = self.first.split_off(self.first.len() / 2);
        let second = self.second.split_off(self.second.len() / 2);
        Numbers { first, second }
    }
}

fn tenpower(number: usize) -> i128 {
    10_i128.pow(number as u32)
}

fn karatsuba(arg1: i128, arg2: i128) -> i128 {
    let mut sign = 1;
    if arg1 < 0 {
        sign = sign * -1
    }
    if arg2 < 0 {
        sign = sign * -1
    }
    sign * karatsuba_impl(Numbers::new(arg1.abs(), arg2.abs()))
}

fn karatsuba_impl(mut numbers: Numbers) -> i128 {
    if numbers.first.len() < 2 {
        let (first, second) = numbers.into_i128();
        return first * second;
    }
    let diff = 10_i128.pow(numbers.make_equal_len() as u32);
    let size = numbers.first.len();

    let bd_arr = numbers.split_in_two();
    let ac_arr = numbers;

    let (b, d) = bd_arr.into_i128();
    let (a, c) = ac_arr.into_i128();
    let abcd_arr = Numbers::new(a + b, c + d);

    let ac = karatsuba_impl(ac_arr);
    let bd = karatsuba_impl(bd_arr);
    let abcd = karatsuba_impl(abcd_arr);
    let fourth_step = abcd - bd - ac;

    (ac * tenpower(size) + bd + fourth_step * tenpower(size / 2)) / diff
}

#[derive(Debug)]
struct VecWrap {
    vector: Vec<u8>,
}

impl VecWrap {
    fn into_wrap(vector: Vec<u8>) -> Self {
        VecWrap { vector }
    }
    fn new() -> Self {
        VecWrap { vector: Vec::new() }
    }
}

impl Add<VecWrap> for VecWrap {
    type Output = VecWrap;
    fn add(self, b: VecWrap) -> VecWrap {
        let a = self.vector;
        let b = b.vector;
        let mut surplus = 0;
        let mut c: Vec<u8> = Vec::new();
        for (i, val_a) in a.iter().rev().enumerate() {
            let mut val_b = 0;
            let b_size = b.len() as i32;
            if b_size - (i as i32) > 0 {
                val_b = b[b.len() - i - 1] - 48;
            }
            let val_a = val_a - 48;
            let mut val_res = val_a + val_b + surplus;
            surplus = 0;
            if val_res > 10 {
                surplus = val_res / 10;
                val_res = val_res % 10;
            }
            c.push(val_res + 48);
        }
        if b.len() > a.len() {
            let d = &b[0..b.len() - a.len()];
            for val in d.iter().rev() {
                let mut res = val - 48 + surplus;
                surplus = 0;
                if res > 10 {
                    surplus = res / 10;
                    res = res % 10;
                }
                c.push(res + 48);
            }
        }
        if surplus != 0 {
            c.push(surplus + 48);
        }
        c.reverse();
        VecWrap::into_wrap(c)
    }
}

fn main() {
    let mut first = String::new();
    let mut second = String::new();
    let _ = io::stdin().read_line(&mut first).unwrap();
    let _ = io::stdin().read_line(&mut second).unwrap();
    // let first = first.trim().parse::<i128>().unwrap();
    // let second = second.trim().parse::<i128>().unwrap();
    let first = VecWrap::into_wrap(first.trim().to_string().into_bytes());
    let second = VecWrap::into_wrap(second.trim().to_string().into_bytes());
    println!("result = {:?}", u8_to_i128(&(first + second).vector));
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn karatsuba_test() {
        assert_eq!(karatsuba(132, 134), 132 * 134);
        assert_eq!(karatsuba(1, 134), 1 * 134);
        assert_eq!(karatsuba(132, 1), 132 * 1);
        assert_eq!(karatsuba(132, 3), 132 * 3);
        assert_eq!(karatsuba(132, 34), 132 * 34);
        assert_eq!(karatsuba(1231233, 1231231232), 1231233 * 1231231232);
        assert_eq!(
            karatsuba(1010101010003103132, 12312123123123213),
            1010101010003103132 * 12312123123123213
        );
        assert_eq!(karatsuba(-2, 1), -2 * 1);
    }
}
