use itertools::Itertools;
use num::integer::lcm;
use std::collections::HashMap;
use std::env;
use std::fs::read_to_string;

#[derive(Debug)]
struct Data {
    steps: String,
    map: HashMap<String, (String, String)>,
    starting_points: Vec<String>,
}

impl Data {
    fn new(path: String) -> Data {
        let input_data = read_to_string(path).unwrap();
        let input_lines: Vec<&str> = input_data.lines().collect();

        let steps = input_lines[0].to_string();

        let map: HashMap<String, (String, String)> = input_lines[2..input_lines.len()]
            .iter()
            .map(|line| {
                line.split(&[' ', '=', ',', '(', ')'])
                    .filter(|x| !x.is_empty())
                    .collect_tuple::<(_, _, _)>()
                    .map(|(step, left, right)| {
                        (step.to_string(), (left.to_string(), right.to_string()))
                    })
                    .unwrap()
            })
            .collect();

        let starting_points: Vec<String> = map
            .keys()
            .filter(|key| key.ends_with("A"))
            .cloned()
            .collect();

        return Data {
            steps,
            map,
            starting_points,
        };
    }

    fn traverse(&self) -> usize {
        let steps: Vec<char> = self.steps.chars().collect();
        let repeat_lens: Vec<usize> = self
            .starting_points
            .iter()
            .map(|start| {
                let mut num_steps_taken = 0;
                let mut current_step = 0;
                let mut current = start.clone();
                while !current.ends_with("Z") {
                    let step = steps[current_step];
                    let options = self.map.get(&current).unwrap();
                    current = match step {
                        'L' => options.0.clone(),
                        'R' => options.1.clone(),
                        _ => unreachable!(),
                    };
                    num_steps_taken += 1;
                    current_step = (current_step + 1) % steps.len();
                }
                num_steps_taken
            })
            .collect();

        return repeat_lens.iter().fold(1, |acc, &next| lcm(acc, next));
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let data = Data::new(args.get(1).unwrap().to_string());
    println!("Solution: {}", data.traverse());
}
