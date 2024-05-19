use core::panic;

#[allow(dead_code)]
#[derive(Debug, PartialEq, Eq, Copy, Clone)]
enum Associativity {
    Left,
    Right,
}

#[derive(Debug)]
struct Operator {
    op: String,
    precedence: u8,
    associativity: Associativity,
}

impl Operator {
    fn create(op: String, precedence: u8) -> Self {
        Operator {
            op,
            precedence,
            associativity: Associativity::Left,
        }
    }
}

struct PolishParser {
    ops: Vec<Operator>,
    out: String,
}

impl PolishParser {
    fn new() -> Self {
        PolishParser {
            ops: Vec::new(),
            out: String::new(),
        }
    }
    fn peek_op(&self) -> Option<u8> {
        if self.ops.is_empty() {
            None
        } else {
            Some(self.ops[self.ops.len() - 1].precedence)
        }
    }
    fn pop_op_while(&mut self, precedence: u8, assoc: Associativity) {
        while let Some(prec) = self.peek_op() {
            if (prec < precedence) || (prec == precedence && assoc == Associativity::Left) {
                self.out.push_str(self.ops.pop().unwrap().op.as_str());
            } else {
                return;
            }
        }
    }
    // 2 * 3 + 1; 1 + 2 * 3; 123*
    fn push_op(&mut self, op: &[u8], precedence: u8, assoc: Associativity) {
        let op = String::from_utf8(op.to_vec()).unwrap();
        self.pop_op_while(precedence, assoc);
        self.ops.push(Operator::create(op, precedence));
    }
    fn opening_bracket(&mut self) {
        let op = String::from('(');
        let precedence = 15;
        self.ops.push(Operator::create(op, precedence));
    }
    fn closing_bracket(&mut self) {
        while let Some(op) = self.ops.pop() {
            if op.op == "(".to_string() {
                return;
            }
            println!("{}", op.op.as_str());
            self.out.push_str(op.op.as_str());
        }
        panic!("wrong stuff");
    }
    fn peek(&self, str: &Vec<u8>, i: usize) -> Option<u8> {
        if i + 1 >= str.len() {
            None
        } else {
            Some(str[i + 1])
        }
    }
    fn parse(&mut self, mut str: Vec<u8>) {
        str = str
            .into_iter()
            .filter(|x| !x.is_ascii_whitespace())
            .collect();
        let mut skip = false;
        let assoc = Associativity::Left;
        for i in 0..str.len() {
            if skip {
                skip = false;
                continue;
            }
            match str[i] {
                b'0'..=b'9' => self.out.push(str[i] as char),
                b'+' => {
                    if i == 0 || str[i - 1] == b'(' {
                        self.push_op(&str[i..=i], 2, assoc);
                    } else {
                        self.push_op(&str[i..=i], 4, assoc)
                    }
                }
                b'-' => {
                    if i == 0 || str[i - 1] == b'(' {
                        self.push_op(&str[i..=i], 2, assoc);
                    } else {
                        self.push_op(&str[i..=i], 4, assoc)
                    }
                }
                b'*' => self.push_op(&str[i..=i], 3, assoc),
                b'%' => self.push_op(&str[i..=i], 3, assoc),
                b'/' => self.push_op(&str[i..=i], 3, assoc),
                b'!' => {
                    if i == 0 || str[i - 1] == b'(' {
                        self.push_op(&str[i..=i], 2, assoc);
                    } else if let Some(x) = self.peek(&str, i) {
                        if x == b'=' {
                            self.push_op(&str[i..=i + 1], 7, assoc);
                            skip = true;
                        }
                    }
                }
                b'~' => {
                    if i == 0 || str[i - 1] == b'(' {
                        self.push_op(&str[i..=i], 2, assoc);
                    }
                }
                b'(' => self.opening_bracket(),
                b')' => self.closing_bracket(),
                b'>' => {
                    if let Some(x) = self.peek(&str, i) {
                        if x == b'>' {
                            self.push_op(&str[i..=i + 1], 5, assoc);
                            skip = true;
                        } else if x == b'=' {
                            self.push_op(&str[i..=i + 1], 6, assoc);
                            skip = true;
                        } else {
                            self.push_op(&str[i..=i], 6, assoc);
                        }
                    } else {
                        self.push_op(&str[i..=i], 6, assoc);
                    }
                }
                b'<' => {
                    if let Some(x) = self.peek(&str, i) {
                        if x == b'<' {
                            self.push_op(&str[i..=i + 1], 5, assoc);
                            skip = true;
                        } else if x == b'=' {
                            self.push_op(&str[i..=i + 1], 6, assoc);
                            skip = true;
                        } else {
                            self.push_op(&str[i..=i], 6, assoc);
                        }
                    } else {
                        self.push_op(&str[i..=i], 6, assoc);
                    }
                }
                b'=' => {
                    if let Some(x) = self.peek(&str, i) {
                        if x == b'=' {
                            self.push_op(&str[i..=i + 1], 7, assoc);
                            skip = true;
                        } else {
                            self.push_op(&str[i..=i], 14, assoc);
                        }
                    } else {
                        self.push_op(&str[i..=i], 14, assoc);
                    }
                }
                b'&' => {
                    if let Some(x) = self.peek(&str, i) {
                        if x == b'&' {
                            self.push_op(&str[i..=i + 1], 11, assoc);
                            skip = true;
                        } else {
                            self.push_op(&str[i..=i], 8, assoc);
                        }
                    }
                }
                b'|' => {
                    if let Some(x) = self.peek(&str, i) {
                        if x == b'|' {
                            self.push_op(&str[i..=i + 1], 12, assoc);
                            skip = true;
                        } else {
                            self.push_op(&str[i..=i], 10, assoc);
                        }
                    }
                }
                b'^' => {
                    if let Some(x) = self.peek(&str, i) {
                        if x == b'^' {
                            self.push_op(&str[i..=i + 1], 10, Associativity::Right);
                        }
                    } else {
                        self.push_op(&str[i..=i], 9, assoc);
                    }
                }
                _ => (),
            }
        }
        self.pop_op_while(15, assoc);
    }
}

fn main() {
    let mut s = PolishParser::new();
    // let str = String::from("((-1 + 2) + 2) * 3");
    let str = String::from("1 || 5 ^^ 3");
    s.parse(str.into_bytes());
    println!("{:?}", s.out);
}
