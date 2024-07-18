import sys


def parse_line(line: str) -> int:

    substrings = line.split(':')
    substrings = substrings[1].strip().split('|')
    winning_numbers = substrings[0].split(' ')
    your_numbers = substrings[1].split(' ')
    num_winners = 0

    for num in your_numbers:
        if num == '':
            continue
        elif num in winning_numbers:
            num_winners += 1
            continue

    return 0 if num_winners == 0 else pow(2, num_winners - 1)


def parse_input(filename: str):
    with open(filename) as f:
        return sum(parse_line(line) for line in f.readlines())

if __name__ == "__main__":
    print(f"Solution: {parse_input(sys.argv[1])}")
