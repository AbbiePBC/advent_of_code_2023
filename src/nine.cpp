// The input contains all possible paths between the points, and we want to visit all paths.
// => get all permutations of points, add up the distances for each path, submit min.
// all paths are bidirectional, but there aren't many -> duplicate info in dict for ease
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> split(std::string line){
    std::vector<std::string> v;
    char delim = ' ';
    char *token = std::strtok(const_cast<char*>(line.c_str()), &delim);
    while (token != nullptr)
    {
        v.push_back(std::string(token));
        token = std::strtok(nullptr, &delim);
    }

    return v;
}


class Paths {
    private:
        std::map<std::string, std::map<std::string, int> > paths;

    public:
        explicit Paths(std::string filename) {

            std::ifstream file(filename);
            if (!file.is_open()) {
                auto err = "Failed to open file: ";
                throw std::invalid_argument(err + filename);
            }

            std::string line;
            while (getline(file, line)) {
                auto vec = split(line);
                auto source = vec[0];
                auto dest = vec[2];
                auto dist = std::stoi(vec[4]);
                paths[source][dest] = dist;
                paths[dest][source] = dist;
            }
            file.close();
        }

        std::pair<int, int> get_solutions(){
            std::vector<std::string> points;
            int shortest = INT_MAX;
            int longest = 0;

            for (const auto& [k, v] : paths) {
                points.push_back(k);
            }

            do {
                int current = 0;
                for (int i = 0; i < points.size() - 1; i ++){
                    current += paths[points[i]][points[i+1]];
                }
                shortest = std::min(current, shortest);
                longest = std::max(current, longest);
            }
            while (std::next_permutation(points.begin(), points.end()));
            return std::pair<int, int>(shortest, longest);
        }


};

int main(int argc, char *argv[]) {

    try {
        auto paths = Paths(argv[1]);
        auto sols = paths.get_solutions();
        std::cout << "Solution 1: " << sols.first << std::endl;
        std::cout << "Solution 2: " << sols.second << std::endl;
        return 0;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

}
