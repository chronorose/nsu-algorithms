#[derive(Debug)]
struct MinStack {
    stack: Vec<(i32, i32)>,
}

impl MinStack {
    fn new() -> Self {
        MinStack { stack: Vec::new() }
    }

    fn push(&mut self, val: i32) {
        let mut min = self.get_min();
        if min > val {
            min = val;
        }
        self.stack.push((val, min));
    }

    fn pop(&mut self) {
        self.stack.pop().unwrap();
    }

    fn top(&self) -> i32 {
        self.stack[self.stack.len() - 1].0
    }

    fn get_min(&self) -> i32 {
        self.stack[self.stack.len() - 1].1
    }
}

fn main() {
    let mut ms = MinStack::new();
    ms.push(-2);
    ms.push(0);
    ms.push(-3);
    println!("{:?}", ms);
    // ms.get_min();
    // ms.pop();
    // ms.top();
    // ms.get_min();
}
