use std::collections::HashMap;
use std::fs::read_to_string;

use itertools::Itertools;
use std::env;

#[derive(Clone, Debug)]
struct Hand {
    set: String,
    initial_rank: i64,
    final_rank: i64,
    value: i64,
}

impl Hand {
    fn new(set: String, value: i64) -> Hand {
        return Hand {
            set,
            value,
            initial_rank: -1,
            final_rank: -1,
        };
    }

    fn set_initial_rank(&mut self) {
        let mut cards_count: HashMap<char, i64> = HashMap::new();
        for card in self.set.chars() {
            *cards_count.entry(card).or_insert(0) += 1;
        }

        let mut num_jokers = 0;
        if cards_count.contains_key(&'J') {
            num_jokers = cards_count.remove(&'J').unwrap();
        }

        let mut count: Vec<i64> = cards_count.into_values().collect::<Vec<i64>>();
        count.sort();
        let num_card_types = count.len();
        if num_card_types == 0 {
            self.initial_rank = 6; // same as setting count to [5]
            return;
        } else {
            count[num_card_types - 1] += num_jokers;
        }

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

    fn get_next(&self, mut char_idx: usize, idxes: Vec<usize>) -> usize {
        let mut char_dict: HashMap<char, Vec<usize>> = HashMap::new();
        for idx in idxes {
            let hand = &self.hands[idx];
            let key: char = hand.set.chars().collect::<Vec<char>>()[char_idx];
            char_dict
                .entry(key)
                .and_modify(|vals| vals.push(idx))
                .or_insert(vec![idx]);
        }

        // sort dict by key value
        let sorted_cards = Self::sort_cards(&char_dict);

        for (_key, vals) in sorted_cards {
            if vals.len() == 1 {
                return vals[0];
            } else {
                char_idx += 1;
                return self.get_next(char_idx, vals.to_vec());
            }
        }
        // throw error or make the return type Optional and return None
        return usize::max_value();
    }

    fn calculate_final_ranks(&mut self) {
        let mut rank: i64 = 1;
        let char_idx: usize = 0;

        let mut initial_rank_to_hand: HashMap<i64, Vec<usize>> = HashMap::new();
        for i in 0..self.hands.len() {
            initial_rank_to_hand
                .entry(self.hands[i].initial_rank)
                .and_modify(|vals| vals.push(i))
                .or_insert(vec![i]);
        }
        for key in initial_rank_to_hand.keys().sorted() {
            let mut to_rank: Vec<usize> = initial_rank_to_hand.get(key).unwrap().to_vec();
            while !to_rank.is_empty() {
                let prev_idx = self.get_next(char_idx, to_rank.clone());
                to_rank.remove(to_rank.iter().position(|x| *x == prev_idx).unwrap());
                self.hands[prev_idx].final_rank = rank;
                rank += 1;
            }
        }
        // for hand in &self.hands {
        //     println!(&hand.set, hand.final_rank, hand.value);
        // }
    }

    fn calculate_solution(self) -> i64 {
        return self
            .hands
            .iter()
            .map(|hand| hand.final_rank * hand.value)
            .sum();
    }
}
fn main() {
    let args: Vec<String> = env::args().collect();
    let input_data = read_to_string(&args[1]).unwrap();
    let mut round = Round::new(&input_data);
    round.calculate_final_ranks();
    println!("Solution: {}", round.calculate_solution());
}
