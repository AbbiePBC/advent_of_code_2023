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
    void traverse(){
        // as we don't know what S connects to, we have to try each path that connects to it until we find one that returns us back to the original destination
        // so, we start at the starting point and try to find a route through
        // if we get stuck before the coordinates are back at the start point, we try another approach
        // possible directions from S = North, South, East, West. 2 of these will produce a correct path
        std::vector<std::pair<int, int> > possible_second_steps;

        if (start_point.first != 0){
            possible_second_steps.push_back(std::pair<int, int> (start_point.first - 1, start_point.second));
        } if (start_point.first != map.size() -1){
            possible_second_steps.push_back(std::pair<int, int> (start_point.first + 1, start_point.second));
        } if (start_point.second != 0){
            possible_second_steps.push_back(std::pair<int, int> (start_point.first, start_point.second + 1));
        } if (start_point.second != map.size() -1) {
            possible_second_steps.push_back(std::pair<int, int> (start_point.first, start_point.second - 1));
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

    int solve(){
        int total_steps = distances[start_point.first][start_point.second];
        return total_steps/2;
    }

};

std::vector<std::vector<char> > Map::map;
std::vector<std::vector<int> > Map::distances;

int main(int argc, char *argv[]) {

    try {
        auto map = Map(argv[1]);
        map.traverse();
        auto sol = map.solve();
        std::cout << "Solution 1: " << sol << std::endl;
        return 0;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

}
