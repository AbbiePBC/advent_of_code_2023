use std::collections::HashMap;
use std::fs::read_to_string;

use itertools::Itertools;
use std::env;

#[derive(Clone, Debug)]
struct Hand {
    set: String,
    initial_rank: i64,
    value: i64,
}

impl Hand {
    fn new(set: String, value: i64) -> Hand {
        return Hand {
            set,
            value,
            initial_rank: -1,
        };
    }

    fn set_initial_rank(&mut self) {
        let mut cards_count: HashMap<char, i64> = HashMap::new();
        for card in self.set.chars() {
            *cards_count.entry(card).or_insert(0) += 1;
        }

        let num_jokers = cards_count.remove(&'J').unwrap_or(0);

        let mut count: Vec<i64> = cards_count.into_values().collect();
        count.sort();

        // Handle cases with jokers more cleanly
        count = match (count.as_slice(), num_jokers) {
            ([], n) => vec![n], // if only jokers, count = [5]

            // learning point: the below arm is the same as the following.
            // count = if num_jokers > 0 {
            //      let mut c = count.to_owned();
            //      c[c.len() - 1] += num_jokers;
            //      c
            // }
            // i.e. you can use val = if statement {...; v} to set val to v.

            (c, n) if n > 0 => { // if jokers present, add to final element of vector
                let mut c = c.to_owned();
                let final_idx = c.len() - 1;
                c[final_idx] += n;
                c
            }

            (c, _) => c.to_owned(), // if n <= 0, i.e. no jokers condition
        };
        self.initial_rank = match count.as_slice() {
            [5] => 6,          // Five of a kind
            [1, 4] => 5,       // Four of a kind
            [2, 3] => 4,       // Full house
            [1, 1, 3] => 3,    // Three of a kind
            [1, 2, 2] => 2,    // Two pair
            [1, 1, 1, 2] => 1, // One pair
            _ => 0,            // High card
        };
    }
}

struct Round {
    hands: Vec<Hand>,
}

impl Round {
    fn new(input: &str) -> Round {
        let input_lines: Vec<&str> = input.split("\n").collect();
        let num_lines = input_lines.len() - 1;

        let mut round: Vec<Hand> = vec![Hand::new("".to_string(), 0); num_lines];
        for i in 0..num_lines {
            if input_lines[i].is_empty() {
                break;
            } else {
                let data: Vec<&str> = input_lines[i].split(" ").collect();
                round[i].set = data[0].to_string();
                round[i].value = data[1].parse::<i64>().unwrap();
                round[i].set_initial_rank();
            }
        }
        return Round { hands: round };
    }

    fn sort_cards(cards: &HashMap<char, Vec<usize>>) -> Vec<(char, Vec<usize>)> {
        let mut card_vec: Vec<(char, Vec<usize>)> =
            cards.iter().map(|(&c, v)| (c, v.clone())).collect();
        card_vec.sort_by_key(|&(c, _)| match c {
            // should extract into a separate fn
            'A' => 12,
            'K' => 11,
            'Q' => 10,
            'T' => 9,
            '9' => 8,
            '8' => 7,
            '7' => 6,
            '6' => 5,
            '5' => 4,
            '4' => 3,
            '3' => 2,
            '2' => 1,
            'J' => 0,
            _ => unreachable!(),
        });
        card_vec
    }

    fn get_next(&self, mut char_idx: usize, idxes: Vec<usize>) -> Option<usize> {
        let mut char_dict: HashMap<char, Vec<usize>> = HashMap::new();
        for idx in idxes {
            let hand = &self.hands[idx];
            let key = hand.set.chars().nth(char_idx)?; // Use nth and Option to handle potential panics
            char_dict.entry(key).or_default().push(idx);
        }

        let sorted_cards = Self::sort_cards(&char_dict);
        for (_key, vals) in sorted_cards {
            return if vals.len() == 1 {
                Some(vals[0])
            } else {
                char_idx += 1;
                self.get_next(char_idx, vals)
            };
        }

        None // Return None if no matching hand is found
    }


    fn calculate_solution(&mut self) -> i64{
        let mut rank = 1;
        let char_idx = 0;
        let mut sol = 0;

        let mut initial_rank_to_hand: HashMap<i64, Vec<usize>> = HashMap::new();
        for (i, hand) in self.hands.iter().enumerate() {
            initial_rank_to_hand
                .entry(hand.initial_rank)
                .or_default()
                .push(i);
        }

        for key in initial_rank_to_hand.keys().sorted() {
            let mut to_rank = initial_rank_to_hand.get(key).unwrap().to_vec();
            while !to_rank.is_empty() {
                let prev_idx = self.get_next(char_idx, to_rank.clone()).unwrap();
                to_rank.remove(to_rank.iter().position(|x| *x == prev_idx).unwrap());
                sol += rank * self.hands[prev_idx].value;
                rank += 1;
            }
        }
        return sol;
    }

}
fn main() {
    let args: Vec<String> = env::args().collect();
    let input_data = read_to_string(&args[1]).unwrap();
    let mut round = Round::new(&input_data);
    println!("Solution: {}", round.calculate_solution());
}
