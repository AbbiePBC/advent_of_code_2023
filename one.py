import sys
import re

def parse_line(line: str) -> int:
    digits = re.findall(r'(?=(one|two|three|four|five|six|seven|eight|nine|[1-9]))', line)
    values = [str(word_to_digit(d)) if d.isalpha() else d for d in digits]
    return int(values[0] + values[-1])

def word_to_digit(word: str) -> int:
    words = {"one": 1, "two": 2, "three": 3, "four": 4, "five": 5, "six": 6, "seven": 7, "eight": 8, "nine": 9}
    return words.get(word, -1)

def parse_input(filename: str):
    with open(filename) as f:
        return sum(parse_line(line) for line in f.readlines())

if __name__ == "__main__":
    print(f"Solution: {parse_input(sys.argv[1])}")
