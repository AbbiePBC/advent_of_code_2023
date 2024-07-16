import sys

def part_one_parse_line(line: str) -> int:
    for char in line:
        if char.isnumeric():
            first_digit = char
            break

    for char in line[::-1]:
        if char.isnumeric():
            last_digit = char
            break

    return int(first_digit)*10 + int(last_digit)

def numeric_word(line: str, idx: int) -> int:
    words = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]
    for w in range(len(words)):
        if line[idx:].startswith(words[w]):
            return w + 1 # 1-indexed
    return -1


def part_two_parse_line(line: str) -> int:

    for i in range(len(line)):
        if line[i].isnumeric():
            first_digit = line[i]
            break
        digit = numeric_word(line, i)
        if digit != -1:
            first_digit = digit
            break

    for i in range(len(line)-1, -1, -1):
        if line[i].isnumeric():
            last_digit = line[i]
            break
        digit = numeric_word(line, i)
        if digit != -1:
            last_digit = digit
            break

    return int(first_digit)*10 + int(last_digit)

def parse_input(filename: str):
    with open(filename) as f:
        lines = f.readlines()
        return sum(part_two_parse_line(line) for line in lines)

if __name__ == "__main__":
    # Assume program is called correctly
    print(f"Solution: {parse_input(sys.argv[1])}")

