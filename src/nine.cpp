#include <fstream>
#include <iostream>
#include <string>
#include <vector>


class Sequence {

    private:
        static bool all_zeroes(const std::vector<int>& seq){
            for (int i : seq){
                if (i != 0){
                    return false;
                }
            }
            return true;
        }

        static int next_in_sequence(std::vector<int> current_sequence){

            std::vector<int> new_seq;
            new_seq.reserve(current_sequence.size() - 1);
            for (int i = 0; i < current_sequence.size() - 1; i ++){
                new_seq.push_back(current_sequence[i+1] - current_sequence[i]);
            }
            if (all_zeroes(new_seq)) {
                return 0;
            } else {
                int next = next_in_sequence(new_seq);
                return new_seq[new_seq.size() -1] + next;
            }
        }

        static std::vector<int> split(const std::string& line){
            std::vector<int> v;
            char delim = ' ';
            char *token = std::strtok(const_cast<char*>(line.c_str()), &delim);
            while (token != nullptr)
            {
                v.push_back(stoi(std::string(token)));
                token = std::strtok(nullptr, &delim);
            }

            return v;
        }

    public:

        static int parse_and_solve(const std::string& filename) {
            std::ifstream file(filename);

            if (!file.is_open()) {
                auto err = "Failed to open file: ";
                throw std::invalid_argument(err + filename);
            }

            std::string line;
            int running_total = 0;
            while (getline(file, line)) {
                std::vector<int> current_sequence = Sequence::split(line);
                int next = next_in_sequence(current_sequence) + current_sequence[current_sequence.size() - 1];
                running_total += next;
            }
            file.close();
            return running_total;
        }

};


int main(int argc, char *argv[]) {

    try {
        auto sol = Sequence::parse_and_solve(argv[1]);
        std::cout << "Solution 1: " << sol << std::endl;
        return 0;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

}
