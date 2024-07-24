from dataclasses import dataclass
import sys

@dataclass
class Scratchcard:
    value: int
    count: int = 1

    @staticmethod
    def parse_line_into_scratchcard(line: str) -> 'Scratchcard':

        _, numbers = line.split(':')
        winning_numbers, your_numbers = numbers.strip().split('|')
        winning_numbers = winning_numbers.split()
        your_numbers = your_numbers.split()
        matching_numbers = set(winning_numbers) & set(your_numbers)

        return Scratchcard(len(matching_numbers))

def parse_input(filename: str) -> list[Scratchcard]:
    scratchcards = []
    with open(filename) as f:
        for line in f.readlines():
            scratchcards.append(Scratchcard.parse_line_into_scratchcard(line))
    return scratchcards

def calculate_score(scratchcards: list[Scratchcard]) -> int:
    score = 0
    for i in range(len(scratchcards)):
        score += scratchcards[i].count
        for j in range(i + 1, i + scratchcards[i].value + 1):
            # Assume no out of bounds error is possible with the given input
            scratchcards[j].count += scratchcards[i].count

    return score

if __name__ == "__main__":
    print(f"Solution: {calculate_score(parse_input(sys.argv[1]))}")
