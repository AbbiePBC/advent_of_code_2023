// In Camel Cards, you get a list of hands, and your goal is to order them based on the strength of each hand.
//A hand consists of five cards labeled one of A, K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, or 2. The relative strength of each card follows this order, where A is the highest and 2 is the lowest.
//
//
// Hands are primarily ordered based on type; for example, every full house is stronger than any three of a kind.
//
// If two hands have the same type, a second ordering rule takes effect.
// Start by comparing the first card in each hand. If these cards are different, the hand with the stronger first card is considered stronger.
// If the first card in each hand have the same label, however, then move on to considering the second card in each hand.
// If they differ, the hand with the higher second card wins; otherwise, continue with the third card in each hand, then the fourth, then the fifth.
//
// So, 33332 and 2AAAA are both four of a kind hands, but 33332 is stronger because its first card is stronger.
// Similarly, 77888 and 77788 are both a full house, but 77888 is stronger because its third card is stronger (and both hands have the same first and second card).
//
// To play Camel Cards, you are given a list of hands and their corresponding bid (your puzzle input). For example:
//
// 32T3K 765
// T55J5 684
// KK677 28
// KTJJT 220
// QQQJA 483
//
// This example shows five hands; each hand is followed by its bid amount. Each hand wins an amount equal to its bid multiplied by its rank, where the weakest hand gets rank 1, the second-weakest hand gets rank 2, and so on up to the strongest hand. Because there are five hands in this example, the strongest hand will have rank 5 and its bid will be multiplied by 5.
//
// So, the first step is to put the hands in order of strength:
//
//     32T3K is the only one pair and the other hands are all a stronger type, so it gets rank 1.
//     KK677 and KTJJT are both two pair. Their first cards both have the same label, but the second card of KK677 is stronger (K vs T), so KTJJT gets rank 2 and KK677 gets rank 3.
//     T55J5 and QQQJA are both three of a kind. QQQJA has a stronger first card, so it gets rank 5 and T55J5 gets rank 4.
//
// Now, you can determine the total winnings of this set of hands by adding up the result of multiplying each hand's bid with its rank (765 * 1 + 220 * 2 + 28 * 3 + 684 * 4 + 483 * 5). So the total winnings in this example are 6440.
//
// Find the rank of every hand in your set. What are the total winnings?

use std::fs::read_to_string;
use std::collections::HashMap;

#[derive(Clone, Debug)]
struct Hand {
    set: String,
    initial_rank: i32,
    final_rank: i32,
    value: i32,
}

impl Hand {
    fn new(set: String, value: i32) -> Hand {
        return Hand {
            set,
            value,
            initial_rank: -1,
            final_rank : -1
        };
    }
    //0     Five of a kind, where all five cards have the same label: AAAAA
    //1     Four of a kind, where four cards have the same label and one card has a different label: AA8AA
    //2     Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
    //3     Three of a kind, where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
    //4     Two pair, where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
    //5     One pair, where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
    //6     High card, where all cards' labels are distinct: 23456
    fn set_initial_rank(&mut self){
        let mut cards_count: HashMap<char, i32> = HashMap::new();
        for card in self.set.chars() {
            *cards_count.entry(card).or_insert(0) += 1;
        }
        let mut count: Vec<i32> = cards_count.into_values().collect::<Vec<i32>>();
            count.sort();
            self.initial_rank = match count.as_slice() {
                [5] => 0, // Five of a kind
                [1, 4] => 1, // Four of a kind
                [2, 3] => 2, // Full house
                [1, 1, 3] => 3, // Three of a kind
                [1, 2, 2] => 4, // Two pair
                [1, 1, 1, 2] => 5, // One pair
                _ => 6, // High card
            };
    }

}

struct Round {
    hands: Vec<Hand>,
}

impl Round {
    fn new(input: &str) -> Round {
        let input_lines: Vec<&str> = input.split("\n").collect();

        let mut round: Vec<Hand> = vec![Hand::new("".to_string(), 0); 4];
        for i in 0..4 {
            if input_lines[i].is_empty() {
                break;
            } else {
                let data: Vec<&str> = input_lines[i].split(" ").collect();
                round[i].set = data[0].to_string();
                round[i].value = data[1].parse::<i32>().unwrap();
                round[i].set_initial_rank();
            }
        }
        return Round { hands : round };
    }

    fn calculate_rank(self) -> i32 {
        return self.hands.iter().map(|hand| hand.final_rank * hand.value).sum()
    }
}
fn main() {
    let input_data = read_to_string("inputs/seven_example.txt").unwrap();
    let mut round = Round::new(&input_data);
    round.calculate_rank();
    println!("{:?}", round.hands);
}
