#![allow(unused)]

#[derive(Debug, Clone)]
struct RedundantCounter {
    t: Vec<usize>,
    s: Vec<usize>,
}

impl std::fmt::Display for RedundantCounter {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let t = &self.t;

        for i in 0..t.capacity() {
            let digit = match t[i] {
                0 => '0',
                1 => '1',
                _ => '2',
            };

            write!(f, "{}", digit)?;
        }

        Ok(())
    }
}

impl RedundantCounter {
    pub fn new(k: usize) -> Self {
        let t = vec![0; k];
        let s = Vec::with_capacity(k);
        Self {t, s}
    }

    pub fn number(&self) -> usize {
        let t = &self.t;

        let mut n = 0;

        for i in 0..t.capacity() {
            n = n + t[i] * usize::pow(2, i as u32);
        }

        n
    }

    fn increment(&mut self) -> &mut Self {
        let t = &mut self.t;
        let s = &mut self.s;

        t[0] = t[0] + 1;

        if t[0] != 2 {
            return self;
        }

        t[0] = 0;

        // fix
        if s.len() == 0 {
            t[1] = t[1] + 1;
            if t[1] == 2 {
                s.push(1);
            }
        } else {
            match t[1] {
                0 => {
                    t[1] = 1;
                },
                1 => {
                    t[1] = 2;
                    let last = s.pop().expect("stack to not be empty");
                    t[last] = 0;
                    t[last + 1] = t[last + 1] + 1;
                    if t[last + 1] == 2 {
                        s.push(last + 1);
                    }
                    s.push(1);
                },
                _ => {
                    s.pop();
                    t[1] = 1;
                    t[2] = t[2] + 1;
                    if t[2] == 2 {
                        s.push(2);
                    }
                },
            }
        }

        self
    }

    fn decrement(&mut self) -> &mut Self {
        let t = &mut self.t;
        let s = &mut self.s;

        if t[0] == 1 {
            t[0] = 0;
            return self;
        }

        // first digit
        t[0] = 1; 
        
        if t[1] == 2 {
            // second digit
            t[1] = 1;
            todo!();

        } else {
            // second digit
            t[1] = 2;

            // third digit
            t[2] = t[2] - 1
        }

        self
    }
}

pub fn test() {
    let mut c = RedundantCounter::new(8);
    println!("-1: {:?}", c);

    for i in 0..256 {
        let prev = c.clone();
        c.increment();

        let mut changed_digits = 0;
        for i in 0..c.t.capacity() {
            if prev.t[i] != c.t[i] {
                changed_digits += 1
            }
        }

        println!(
            "{}: {}, {} | {} | {:?}",
            i,
            c.number(),
            c,
            changed_digits,
            c.s,
        );
    }
}
