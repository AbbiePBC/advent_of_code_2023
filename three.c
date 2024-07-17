#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct PartNumber {
    int num;
    int row;
    int start_idx;
    int end_idx;
} PartNumber;

typedef struct Symbol {
    int row;
    int column;
} Symbol;

int min(int a, int b){
    return a < b ? a : b;
}

bool is_part_number(PartNumber part, Symbol *symbols, int num_symbols){

    for (int i = 0; i < num_symbols; i ++){

        int dist_to_start_idx = part.start_idx - symbols[i].column;
        int dist_to_end_idx = part.end_idx - symbols[i].column;
        int diff = min(abs(dist_to_start_idx), abs(dist_to_end_idx));
        bool adjacent_x = diff <=1;
        bool adjacent_y = abs(part.row - symbols[i].row) <=1;
        if (adjacent_x && adjacent_y){
            return true;
        }
    }
    return false;
}

int calculate_sum(FILE* fptr){

    size_t len = 0;
    char *line = 0;
    ssize_t nread;
    int row = 0;
    // Allocate memory (trial and error size)
    int MAX_MEM_SIZE = 2500;
    PartNumber *parts = malloc(sizeof(PartNumber *) * MAX_MEM_SIZE);
    Symbol *symbols = malloc(sizeof(Symbol *) * MAX_MEM_SIZE);
    if (parts == NULL || symbols == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    int part_numbers_found = 0;
    int symbols_found = 0;
    while ((nread = getline(&line, &len, fptr)) != -1) {

        int num = 0;
        int start_idx = 0;
        for (int i = 0; i < nread; i ++){
            char c = line[i];
            if (c > '9' || c < '0' ){
                if (num != 0){
                    // Flush part number to struct
                    PartNumber p = {num, row, start_idx, i-1};
                    parts[part_numbers_found] = p;
                    part_numbers_found ++;
                    num = 0;
                }

                if (c != '.' && c != '\0' && c != '\n'){
                    // Symbol found
                    Symbol s = {row, i};
                    symbols[symbols_found] = s;
                    symbols_found ++;
                }
            } else {
                // Begin new number
                if (num == 0){
                    start_idx = i;
                }
                num *= 10;
                num += (c - '0');
            }
        }
        row ++;
    }
    // Calculate sum
    int sum = 0;
    for (int p = 0; p < part_numbers_found; p ++){
        if (is_part_number(parts[p], symbols, symbols_found)){
            sum += parts[p].num;
        }
    }

    free(parts);
    free(symbols);
    return sum;
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *fptr;
    fptr = fopen(argv[1], "r");

    if(fptr != NULL) {
        int sum = calculate_sum(fptr);
        printf("Solution: %d\n", sum);
        fclose(fptr);
    } else {
        printf("Unable to open the file.");
    }
}
