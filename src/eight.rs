use std::collections::HashMap;
use std::fs::read_to_string;
use std::env;

use itertools::Itertools;

#[derive(Debug)]
struct Data {
    steps: String,
    map: HashMap<String, (String, String)>,
    starting_points: Vec<String>,
}

impl Data {
    fn new(path: String) -> Data {
        let input_data = read_to_string(path).unwrap();
        let input_lines: Vec<&str> = input_data.split("\n").collect();

        let steps = input_lines[0].to_string();
        let mut map: HashMap<String, (String, String)> = HashMap::new();
        let mut starting_points : Vec<String> = Vec::new();
        for i in 2..input_lines.len() - 1 {
            let vals: Vec<&str> = input_lines[i]
                .split(&[' ', '=', ',', '(', ')'])
                .filter(|x| !x.is_empty())
                .collect();
            let step = vals[0].to_string();
            map.insert(
                step.clone(),
                (vals[1].to_string(), vals[2].to_string()),
            );
            if step.to_string().ends_with("A"){
                starting_points.push(step);
            }
        }
        return Data { steps, map, starting_points };
    }

    fn single_step(&mut self, step: char, idx: usize) -> bool {

        let mut current = &self.starting_points[idx];
        let options = self.map.get(current).unwrap();
        current = match step {
            'L' => &options.0,
            'R' => &options.1,
            _ => unreachable!(),
        };
        self.starting_points[idx] = current.clone();
        return current.ends_with("Z")
    }

    fn multi_traverse(&mut self) -> usize {
        let mut num_steps_taken: usize = 0;
        let mut current_step = 0;
        let steps: Vec<char> = self.steps.chars().collect_vec();
        let mut all_complete: bool = false;
        let num_journeys = self.starting_points.len();
        while !all_complete {

            all_complete = true;
            let step = steps[current_step];
            for i in 0..num_journeys {
                let complete = self.single_step(step, i);
                if !complete{
                    all_complete = false;
                }
            }

            num_steps_taken += 1;
            println!("steps taken = {}", num_steps_taken);
            current_step = (current_step + 1) % steps.len();
        }

        return num_steps_taken;
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    let mut data = Data::new(args.get(1).unwrap().to_string());
    println!("Solution: {}", data.multi_traverse());
}
