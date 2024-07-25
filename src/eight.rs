use std::collections::HashMap;
use std::fs::read_to_string;

use std::env;

use itertools::Itertools;

#[derive(Debug)]
struct Data {
    steps: String,
    map: HashMap<String, (String, String)>,
}

impl Data {
    fn new(path: String) -> Data {
        let input_data = read_to_string(path).unwrap();
        let input_lines: Vec<&str> = input_data.split("\n").collect();

        let steps = input_lines[0].to_string();
        let mut map: HashMap<String, (String, String)> = HashMap::new();
        for i in 2..input_lines.len() - 1 {
            let vals: Vec<&str> = input_lines[i]
                .split(&[' ', '=', ',', '(', ')'])
                .filter(|x| !x.is_empty())
                .collect();
            map.insert(
                vals[0].to_string(),
                (vals[1].to_string(), vals[2].to_string()),
            );
        }
        return Data { steps, map };
    }

    fn traverse(&self) -> usize {
        let mut num_steps_taken: usize = 0;
        let mut current_step = 0;
        let mut current: String = "AAA".to_string();
        let steps: Vec<char> = self.steps.chars().collect_vec();
        while current != "ZZZ".to_string() {
            let step = steps[current_step];
            let options = self.map.get(&current).unwrap();
            current = match step {
                'L' => options.0.to_string(),
                'R' => options.1.to_string(),
                _ => unreachable!(),
            };
            num_steps_taken += 1;
            current_step = (current_step + 1) % steps.len();
        }
        return num_steps_taken;
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let data = Data::new(args.get(1).unwrap().to_string());
    println!("Solution: {}", data.traverse());
}
