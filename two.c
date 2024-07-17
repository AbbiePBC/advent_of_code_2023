#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct Colour{
    const char* name;
    int max_number_in_game;
    const int max_allowed;
};

int max(int a, int b){
    return a > b ? a : b;
}

void update_max(char* token, struct Colour *colours, const int NUM_COLOURS){
    int number = 0;
    char name[10]; // should really use malloc to prevent buffer overflow
    int result = sscanf(token, "%d %s", &number, name);
    if (result == 2){ // if number and name are both found
        for (int i = 0; i < NUM_COLOURS; i ++){
            if (strstr(colours[i].name, name)){
                colours[i].max_number_in_game = max(number, colours[i].max_number_in_game);
                break;
            }
        }
    }
}

bool is_valid_game(struct Colour *colours, const int NUM_COLOURS){
    for (int i = 0; i < NUM_COLOURS; i ++){
        if (colours[i].max_number_in_game > colours[i].max_allowed){
            return false;
        }
    }
    return true;
}

/**
 * Part 1: parses the line to determine whether the game is valid,
 * and therefore whether to add the line_number to the sum
 */
bool parse_line_to_bool(const char* line, struct Colour* colours, const int num_colours){


    char *token;
    token = strtok(line, ":");

    for (int i = 0; i < num_colours; i ++){
        colours[i].max_number_in_game = 0;
    }

    while( token != NULL ) {
        token = strtok(NULL, ",;");
        if (!token){
            break;
        }
        update_max(token, colours, num_colours);
    }
    if (is_valid_game(colours, num_colours)){
        return true;
    }
    return false;
}

int calculate_game_score(FILE* fptr){
    const int LINE_LEN = 200; // should really use malloc to prevent buffer overflow

    char input[LINE_LEN];

    // Example input
    // Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
    int sum = 0;
    const int NUM_COLOURS = 3;
    struct Colour colours[NUM_COLOURS] = {
            {"red", 0, 12},
            {"green", 0, 13},
            {"blue", 0, 14}
    };
    int line_number = 0;
    while(fgets(input, LINE_LEN, fptr)) {
        line_number++;
        if (parse_line_to_bool(input, colours, NUM_COLOURS)){
            sum += line_number;
        }

    }
    return sum;
}

int main(int argc, char *argv[]) {
    FILE *fptr;
    fptr = fopen(argv[1], "r");

    if(fptr != NULL) {
        int sum = calculate_game_score(fptr);
        printf("Solution: %d\n", sum);
    } else {
        printf("Unable to open the file.");
    }
    fclose(fptr);
}
