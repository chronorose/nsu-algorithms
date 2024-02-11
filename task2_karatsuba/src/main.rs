#![allow(dead_code)]
use core::fmt;
use std::{
    cmp, io,
    ops::{Add, Mul, Sub},
};

fn zero_last_places(vector: &mut Vec<u8>, new_size: usize) -> i32 {
    let mut amount: i32 = 0;
    while vector.len() < new_size {
        vector.push(48);
        amount += 1;
    }
    amount
}

fn karatsuba(arg1: BigInt, arg2: BigInt) -> BigInt {
    karatsuba_impl(arg1, arg2)
}

fn karatsuba_impl(mut first: BigInt, mut second: BigInt) -> BigInt {
    if first.vector.len() < 2 || second.vector.len() < 2 {
        return first * second;
    }
    let diff = first.make_equal_len(&mut second);
    let size = first.vector.len();

    let b = first.split_in_two();
    let a = first;
    let d = second.split_in_two();
    let c = second;

    let ab = a.clone() + b.clone();
    let cd = c.clone() + d.clone();
    let mut ac = karatsuba_impl(a, c);
    let bd = karatsuba_impl(b, d);
    let abcd = karatsuba_impl(ab, cd);
    let mut fourth_step = abcd - bd.clone() - ac.clone();
    let mut last_step = ac.pow(size) + bd.clone() + fourth_step.pow(size / 2);
    for _ in 0..diff {
        last_step.vector.pop();
    }
    last_step
}

#[derive(Debug, Clone)]
struct BigInt {
    vector: Vec<u8>,
}

impl BigInt {
    fn to_string(self) -> String {
        let mut new = String::new();
        for i in self.vector.iter() {
            new.push_str(&i.to_string());
        }
        new
    }
    fn into_big_int(vector: Vec<u8>) -> Self {
        let big_int = BigInt { vector };
        match big_int.check_correctness() {
            Ok(sl) => sl,
            Err(err) => {
                println!("{}", err);
                BigInt::zero()
            }
        }
    }
    fn from_str(string: &str) -> Self {
        let big_int = BigInt {
            vector: string.to_string().into_bytes(),
        };
        big_int.check_correctness().unwrap()
    }
    fn new() -> Self {
        BigInt { vector: Vec::new() }
    }
    fn check_correctness(mut self) -> Result<Self, Self> {
        let mut non_zero = false;
        self.vector.reverse();
        for i in self.vector.clone().iter().rev() {
            if !non_zero {
                if self.vector.len() > 1 && *i == 48 {
                    self.vector.pop();
                } else {
                    non_zero = true;
                }
            }
            match *i {
                48..=57 => continue,
                _ => return Err(self),
            }
        }
        self.vector.reverse();
        Ok(self)
    }
    fn is_empty(&self) -> bool {
        if self.vector.len() == 0 {
            true
        } else {
            false
        }
    }
    fn split_in_two(&mut self) -> BigInt {
        BigInt::into_big_int(self.vector.split_off(self.vector.len() / 2))
    }
    fn pow(&mut self, power: usize) -> Self {
        for _ in 0..power {
            self.vector.push(48);
        }
        self.clone()
    }
    fn make_equal_len(&mut self, other: &mut BigInt) -> i32 {
        let mut new_len = cmp::max(self.vector.len(), other.vector.len());
        if new_len % 2 != 0 {
            new_len += 1;
        }
        let first_diff = zero_last_places(&mut self.vector, new_len);
        let second_diff = zero_last_places(&mut other.vector, new_len);
        first_diff + second_diff
    }
    fn zero() -> Self {
        BigInt { vector: vec![48] }
    }
    fn more(self, other: BigInt) -> bool {
        let first = self.vector;
        let other = other.vector;
        if first.len() > other.len() {
            true
        } else if other.len() > first.len() {
            false
        } else {
            let mut ret = false;
            for (i, val) in first.iter().enumerate() {
                if *val > other[i] {
                    ret = true;
                    break;
                } else if *val < other[i] {
                    break;
                }
            }
            ret
        }
    }
}

fn trim_right(vector: &mut Vec<u8>) {
    let mut i = vector.len() - 1;
    while i > 0 {
        if vector[i] != 48 {
            break;
        }
        vector.pop();
        i -= 1;
    }
}

impl fmt::Display for BigInt {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let mut str = String::new();
        for i in self.vector.iter() {
            str.push_str(&i.escape_ascii().to_string());
        }
        write!(f, "{}", str)
    }
}

impl Mul<BigInt> for BigInt {
    type Output = BigInt;
    fn mul(self, b: BigInt) -> Self::Output {
        let a = self.vector;
        let b = b.vector;
        let mut c = BigInt::zero();
        let mut surplus = 0;
        let mut endpoint = false;
        for (i, val_b) in b.iter().rev().enumerate() {
            let mut sub_res = BigInt::new();
            for _ in 0..i {
                sub_res.vector.push(48);
            }
            for val_a in a.iter().rev() {
                let mut val_res = (val_a - 48) * (val_b - 48) + surplus;
                surplus = 0;
                endpoint = false;
                if val_res >= 10 {
                    surplus = val_res / 10;
                    val_res = val_res % 10;
                    if val_res == 0 {
                        endpoint = true;
                    }
                }
                sub_res.vector.push(val_res + 48);
            }
            sub_res.vector.reverse();
            c = c + sub_res;
        }
        if surplus != 0 {
            let mut surplus_vec = Vec::new();
            let mut i = 0;
            let size = if endpoint {
                c.vector.len() + 1
            } else {
                c.vector.len()
            };
            while i < size {
                surplus_vec.push(48);
                i += 1;
            }
            surplus_vec.push(surplus + 48);
            surplus_vec.reverse();
            c = c + BigInt::into_big_int(surplus_vec);
        }
        c.check_correctness().unwrap()
    }
}

impl Add<BigInt> for BigInt {
    type Output = BigInt;
    fn add(self, b: BigInt) -> Self::Output {
        let a = self.vector;
        let mut b = b.vector;
        b.reverse();
        let mut surplus = 0;
        let mut c: Vec<u8> = Vec::new();
        for (i, val_a) in a.iter().rev().enumerate() {
            let mut val_b = 0;
            if b.len() > i {
                val_b = b[i] - 48;
            }
            let val_a = val_a - 48;
            let mut val_res = val_a + val_b + surplus;
            surplus = 0;
            if val_res >= 10 {
                surplus = val_res / 10;
                val_res = val_res % 10;
            }
            c.push(val_res + 48);
        }
        if b.len() > a.len() {
            let d = &b[a.len()..b.len()];
            for val in d.iter() {
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
        BigInt::into_big_int(c)
    }
}

impl Sub<BigInt> for BigInt {
    type Output = BigInt;
    fn sub(self, b: BigInt) -> Self::Output {
        let mut a = self.vector.clone();
        a.reverse();
        let b = b.vector.clone();
        let mut c: Vec<u8> = Vec::new();
        let mut endpoint = 0;
        for (i, val_b) in b.iter().rev().enumerate() {
            endpoint = i + 1;
            let val_a = a[i];
            let val_res = if *val_b > val_a && i < a.len() {
                let mut j = i + 1;
                while j < a.len() && a[j] == 48 {
                    j += 1;
                }
                a[j] -= 1;
                j -= 1;
                while j > i {
                    a[j] = 57;
                    j -= 1;
                }
                (val_a - 48 + 10) - (val_b - 48)
            } else {
                (val_a - 48) - (val_b - 48)
            };
            c.push(val_res + 48);
        }
        while endpoint < a.len() {
            c.push(a[endpoint]);
            endpoint += 1;
        }
        trim_right(&mut c);
        c.reverse();
        BigInt::into_big_int(c)
    }
}

fn main() {
    let mut first = String::new();
    let mut second = String::new();
    let _ = io::stdin().read_line(&mut first).unwrap();
    let _ = io::stdin().read_line(&mut second).unwrap();
    let first1 = BigInt::into_big_int(first.clone().trim().to_string().into_bytes());
    let second = BigInt::into_big_int(second.trim().to_string().into_bytes());
    println!("result = {}", karatsuba(first1, second));
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn karatsuba_test() {
        assert_eq!(
            kar_bi("132", "134").to_string(),
            stolb("132", "134").to_string()
        );
        assert_eq!(
            kar_bi("1", "134").to_string(),
            stolb("1", "134").to_string()
        );
        assert_eq!(
            kar_bi("132", "3").to_string(),
            stolb("132", "3").to_string()
        );
        assert_eq!(
            kar_bi("1331331313131313132", "123123").to_string(),
            stolb("1331331313131313132", "123123").to_string()
        );
    }
    fn kar_bi(first: &str, second: &str) -> BigInt {
        karatsuba(BigInt::from_str(first), BigInt::from_str(second))
    }
    fn stolb(first: &str, second: &str) -> BigInt {
        BigInt::from_str(first) * BigInt::from_str(second)
    }
}
