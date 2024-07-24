#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct Colour{
    const char* name;
    int max_number_in_game;
};

int max(int a, int b){
    return a > b ? a : b;
}

void update_max(const char* token, struct Colour *colours, const int NUM_COLOURS){
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

/**
 * Part 2: parses the line to determine the lowest possible number of each colour,
 * (i.e. the maximum of each colour found) and multiplies the values together
 */
int parse_line_to_int(char* line, struct Colour* colours, const int NUM_COLOURS){

    char *token;
    token = strtok(line, ":");

    for (int i = 0; i < NUM_COLOURS; i ++){
        colours[i].max_number_in_game = 0;
    }

    while( token != NULL ) {
        token = strtok(NULL, ",;");
        if (!token){
            break;
        }
        update_max(token, colours, NUM_COLOURS);
    }
    int line_value = 1;
    for (int i = 0; i < NUM_COLOURS; i ++){
        line_value *= colours[i].max_number_in_game;
    }
    return line_value;
}

int calculate_game_score(FILE* fptr){
    const int LINE_LEN = 200; // should really use malloc to prevent buffer overflow

    char input[LINE_LEN];

    // Example input
    // Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
    int sum = 0;
    const int NUM_COLOURS = 3;
    struct Colour colours[NUM_COLOURS] = {
            {"red", 0},
            {"green", 0},
            {"blue", 0}
    };
    while(fgets(input, LINE_LEN, fptr)) {
        sum += parse_line_to_int(input, colours, NUM_COLOURS);
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
