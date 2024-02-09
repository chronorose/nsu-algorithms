#[derive(Debug, Clone)]
struct Numbers {
    first: Vec<u8>,
    second: Vec<u8>,
}

fn u8_to_i32(number: &Vec<u8>) -> i32 {
    let mut first = 0;
    for (i, val) in number.into_iter().rev().enumerate() {
        first += 10_i32.pow(i as u32) * ((*val as i32) - 48);
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
        let mut new_len = std::cmp::max(self.first.len(), self.second.len());
        if new_len % 2 != 0 {
            new_len += 1;
        }
        let first_diff = zero_last_places(&mut self.first, new_len);
        let second_diff = zero_last_places(&mut self.second, new_len);
        first_diff + second_diff
    }
    //     let ln1 = self.first.len() as i32;
    //     let ln2 = self.second.len() as i32;
    //     let mut diff = (ln1 - ln2).abs();
    //     if ln1 > ln2 {
    //         for _ in 0..diff {
    //             self.second.push(48);
    //         }
    //         if ln1 % 2 != 0 {
    //             self.first.push(48);
    //             self.second.push(48);
    //             diff += 2;
    //         }
    //     } else if ln2 > ln1 {
    //         for _ in 0..diff {
    //             self.first.push(48);
    //         }
    //         if ln2 % 2 != 0 {
    //             self.first.push(48);
    //             self.second.push(48);
    //             diff += 2;
    //         }
    //     }
    //     diff
    // }
    fn into_i32(&self) -> (i32, i32) {
        (u8_to_i32(&self.first), u8_to_i32(&self.second))
    }
    fn new(first: i32, second: i32) -> Self {
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

fn karatsuba(arg1: i32, arg2: i32) -> i32 {
    karatsuba_impl(Numbers::new(arg1, arg2))
}

fn karatsuba_impl(mut numbers: Numbers) -> i32 {
    if numbers.first.len() < 2 {
        let (first, second) = numbers.into_i32();
        return first * second;
    }
    let diff = 10_i32.pow(numbers.make_equal_len() as u32);
    let n = 10_i32.pow(numbers.first.len() as u32);
    let n2 = 10_i32.pow((numbers.first.len() / 2) as u32);
    let bd_arr = numbers.split_in_two();
    let ac_arr = numbers;
    let (b, d) = bd_arr.into_i32();
    let (a, c) = ac_arr.into_i32();
    let abcd_arr = Numbers::new(a + b, c + d);
    let ac = karatsuba_impl(ac_arr.clone());
    let bd = karatsuba_impl(bd_arr.clone());
    let abcd = karatsuba_impl(abcd_arr.clone());
    let fourth_step = abcd - bd - ac;
    let fifth = (ac * n + bd + fourth_step * n2) / diff;
    fifth
}

fn main() {
    println!("{}", karatsuba(1234, 5678));
}

#[cfg(test)]
mod tests {}
