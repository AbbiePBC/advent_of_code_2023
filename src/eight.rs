use std::collections::HashMap;
use std::fs::read_to_string;
use std::env;
use num::integer::lcm;
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

    fn traverse(&self) -> usize {
        let mut num_steps_taken: usize;
        let mut current_step: usize;
        let steps: Vec<char> = self.steps.chars().collect_vec();
        let num_journeys: usize = self.starting_points.len();
        let mut repeat_lens: Vec<usize> = Vec::new();
        // TODO: there are more efficient ways of doing this, e.g. the loops the other way around
        // as we then don't have to get the step as often, but it'll do§
        for i in 0..num_journeys{
            num_steps_taken = 0;
            current_step = 0;
            let mut current = self.starting_points[i].clone();
            while !current.ends_with("Z") {
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
            repeat_lens.push(num_steps_taken);
        }

        let mut sol = repeat_lens[0];
        for i in 0..repeat_lens.len() - 1 {
            sol = lcm(sol, repeat_lens[i+1]);
        }

        return sol;
    }

}

fn main() {
    let args: Vec<String> = env::args().collect();
    let data = Data::new(args.get(1).unwrap().to_string());
    println!("Solution: {}", data.traverse());
}
