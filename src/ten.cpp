#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Map {

    private:
        static std::vector<std::vector<char> > map;
        static std::vector<std::vector<int> > distances;
        std::pair<int, int> start_point;

        void print(std::vector<std::vector<char> > map){
            for (auto row : map){
                for (auto ch : row){
                    std::cout << ch;
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
            std::cout << "starting at: (" << start_point.first << ", " << start_point.second << ")." << std::endl;
        }
        void print(std::vector<std::vector<int> > map){
            for (auto row : map){
                for (auto ch : row){
                    std::cout << ch << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }


        std::pair<int, int> take_step(std::pair<int, int>& current_step, int step_count){
            auto y = current_step.first;
            auto x = current_step.second;
            // some of these if statements can be simplified
            // and this assumes we don't risk leaving the map (i.e. there isn't a | at the top for example)
            switch (map[y][x]){
                case '|':
                    if (distances[y-1][x] == -1 || (distances[y-1][x] == 0 && step_count > 2)){
                        return std::pair<int, int>(y - 1, x);
                    } else {
                        return std::pair<int, int>(y + 1, x);
                    }
                case '-':
                    if (distances[y][x-1] == -1 || (distances[y][x-1] == 0 && step_count > 2)){
                        return std::pair<int, int>(y, x - 1);
                    } else {
                        return std::pair<int, int>(y, x + 1);
                    }
                case 'L':
                    if (distances[y][x+1] != -1 &&  (distances[y-1][x] == -1 || distances[y-1][x] ==0)){
                        return std::pair<int, int>(y-1, x);
                    } else if (distances[y][x+1] == -1 || distances[y][x+1]  == 0 &&  distances[y-1][x] != -1){
                        return std::pair<int, int>(y, x+1);
                    } else {
                        throw std::exception();
                    }
                case 'J':
                    if (distances[y][x-1] != -1 &&  (distances[y-1][x] == -1 || distances[y-1][x] == 0 )){
                        return std::pair<int, int>(y-1, x);
                    } else if ((distances[y][x-1] == -1 || distances[y][x-1] == 0) &&  distances[y-1][x] != -1){
                        return std::pair<int, int>(y, x-1);
                    }
                case '7':
                    if (distances[y][x-1] != -1 &&  (distances[y+1][x] == -1 || distances[y+1][x] == 0)){
                        return std::pair<int, int>(y+1, x);
                    } else if ((distances[y][x-1] == -1 || distances[y][x-1] == 0) &&  distances[y+1][x] != -1){
                        return std::pair<int, int>(y, x-1);
                    }
                case 'F':
                    if (distances[y][x+1] != -1 &&  (distances[y+1][x] == -1 || distances[y+1][x] == 0)){
                        return std::pair<int, int>(y+1, x);
                    } else if ((distances[y][x+1] == -1 || distances[y][x+1] == 0) &&  distances[y+1][x] != -1){
                        return std::pair<int, int>(y, x+1);
                    }
            }
            throw std::exception();

        }

        char start_pipe(){
            // find positions of piece 1 and max_dist -1
            std::pair<int, int> min_coords;
            std::pair<int, int> max_coords;

            for (int i = -1; i < 2; i ++){
                for (int j = -1; j < 2; j ++){
                    int x_coord = start_point.second + j;
                    int y_coord = start_point.first + i;
                    if (x_coord < 0 || y_coord < 0 || x_coord >= map.size() || y_coord >= map.size()){
                        continue;
                    }
                    if (distances[y_coord][x_coord] == 1){
                        min_coords = std::pair<int, int>(start_point.first + i, x_coord);
                    }
                    if (distances[y_coord][x_coord] == distances[start_point.first][start_point.second] - 1){
                        max_coords = std::pair<int, int>(y_coord, x_coord);
                    }
                }
            }
            int change_in_y = max_coords.first - min_coords.first;
            int change_in_x = max_coords.second - min_coords.second;

            if (change_in_x == 0 && change_in_y == 2){
                return '|';
            } else if (change_in_x == 2 && change_in_y == 0){
                return '-';
            }

            int gradient = change_in_y / change_in_x;
            int mid_y = (min_coords.second + max_coords.second)/2;
            if (gradient == -1){
                // '/' options, i.e. 'J', 'F'
                if (start_point.second < mid_y){ // remember y is in opposite direction to expected
                    // "under" the '/'
                    return 'F';
                } else return 'J';
            } else if (gradient == 1){
                // '\' options, i.e. '7', 'L'
                if (start_point.second < mid_y){
                    // "under" the '\'
                    return '7';
                } else return 'L';
            }
        }

        void update_map(){
            map[start_point.first][start_point.second] = start_pipe();
            std::cout << "S replaced by " << map[start_point.first][start_point.second] << std::endl;
        }

    public:

        Map(const std::string& filename) {
            std::ifstream file(filename);

            if (!file.is_open()) {
                auto err = "Failed to open file: ";
                throw std::invalid_argument(err + filename);
            }

            std::string line;
            int line_count = 0;
            while (getline(file, line)){
                std::vector<char> line_vec;
                int char_count = 0;
                for (auto ch : line){

                    line_vec.push_back(ch);
                    if (ch == 'S'){
                        start_point = std::pair<int, int>(line_count, char_count);
                    }
                    char_count ++;
                }
                line_count ++;
                map.push_back(line_vec);
                distances.push_back(std::vector<int>(line_vec.size(), -1)); // square input
            }
            //print(map);
            file.close();
        }

        void traverse(){
            // as we don't know what S connects to, we have to try each path that connects to it until we find one that returns us back to the original destination
            // so, we start at the starting point and try to find a route through
            // if we get stuck before the coordinates are back at the start point, we try another approach
            // possible directions from S = North, South, East, West. 2 of these will produce a correct path
            std::vector<std::pair<int, int> > possible_second_steps;

            if (start_point.first != 0 ){
                int new_y = start_point.first - 1;
                int new_x = start_point.second;
                if (map[new_y][new_x] == '|' || map[new_y][new_x] == '7' || map[new_y][new_x] == 'F'){
                    // to be able to start one above, that value should be '|' or '7' or 'F'
                    std::cout<< "try starting at " << new_y << ", "<< new_x << std::endl;
                    possible_second_steps.push_back(std::pair<int, int> (new_y, new_x));
                }
            } if (start_point.first != map.size() -1){
                int new_y = start_point.first + 1;
                int new_x = start_point.second;
                if (map[new_y][new_x] == '|' || map[new_y][new_x] == 'L' || map[new_y][new_x] == 'J') {
                    // to be able to start one below, that value should be '|' or 'L' or 'J'
                    possible_second_steps.push_back(std::pair<int, int>(new_y, new_x));
                }
            } if (start_point.second != 0){
                int new_y = start_point.first;
                int new_x = start_point.second - 1;
                if (map[new_y][new_x] == '|' || map[new_y][new_x] == 'J' || map[new_y][new_x] == '7') {
                    // to be able to start one left, that value should be '-' or 'J' or '7'
                    possible_second_steps.push_back(std::pair<int, int>(new_y, new_x));
                }
            } if (start_point.second != map.size() -1) {
                int new_y = start_point.first;
                int new_x = start_point.second + 1;
                if (map[new_y][new_x] == '|' || map[new_y][new_x] == 'F' || map[new_y][new_x] == 'J') {
                    // to be able to start one right, that value should be '-' or 'F' or 'J'
                    possible_second_steps.push_back(std::pair<int, int>(new_y, new_x));
                }
            }
            for (int i = 0; i < possible_second_steps.size() - 1; i ++){
                int step_count = 1;
                auto current_step = possible_second_steps[i];
                distances[current_step.first][current_step.second] = step_count;
                distances[start_point.first][start_point.second] = 0;

                try {
                    while (current_step != start_point) {
                        step_count ++;
                        current_step = take_step(current_step, step_count);
                        distances[current_step.first][current_step.second] = step_count;
                    }
                    print(distances);
                    return;

                }
                catch (std::exception &e) {
                    std::cout <<"Invalid starting direction. Resetting..."<< std::endl;
                    for (int i = 0; i < distances[0].size(); i ++){
                        for (int j = 0; j < distances[0].size(); j ++){
                            distances[i][j] = -1;
                        }
                    }
                    distances[start_point.first][start_point.second] = 0;
                }
            }

        }

        int solve_steps(){
            int total_steps = distances[start_point.first][start_point.second];
            return total_steps/2;
        }

        int inside_line(int y){
            int num_inside_line = 0;
            bool inside_loop = false;
            char prev = '.';
            auto line = map[y];
            for (int j = 0; j < map.size(); j ++){ // assumes square input
                char current = line[j];
                if (distances[y][j] != -1){
                    // on the line
                    if (current == '|'){
                        prev = current;
                        inside_loop = !inside_loop;

                        continue;
                    } else if (current == 'J' || current == '7' || current == 'F' || current == 'L'){
                        // because L7 makes a corner and LJ makes a U-turn
                        // therefore we need to keep track of the prev ch, and if it makes a corner, we don't flip the inside_loop bool again
                        std::cout << "prev = " <<prev << " current " << current << std::endl;
                        if ((prev == 'L' && current == '7') ||
                            (prev == 'F' && current == 'J')) {
                            prev = current;
                            continue;
                        } else {
                            inside_loop = !inside_loop;
                            prev = current;
                            continue;
                        }
                    }
                }
                if (distances[y][j] == -1 && inside_loop){
                    std::cout << "inside loop at " << y << ", " << j << " for char " << current << std::endl;
                    num_inside_line ++;
                }
                // else, part of loop that doesn't count, e.g. '-', or non-loop piping.
            }
            return num_inside_line;
        }

        int solve_fill(){
            update_map();
            // iterate through the map, top to bottom, left to right
            // depending on the char and the distance, determine whether inside or outside loop
            // if cross loop, change state
            int num_inside_loop = 0;
            for (int i = 0; i < map.size(); i ++){
                num_inside_loop += inside_line(i);
            }
            return num_inside_loop;
        }



};

std::vector<std::vector<char> > Map::map;
std::vector<std::vector<int> > Map::distances;

int main(int argc, char *argv[]) {

    try {
        auto map = Map(argv[1]);
        map.traverse();
        std::cout << "Solution 1: " << map.solve_steps() << std::endl;
        std::cout << "Solution 2: " << map.solve_fill() << std::endl;
        return 0;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

}
