#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <chrono>
#include <string>

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

void write_prime_twins_to_file(const std::vector<std::pair<int, int>>& prime_twins, int& global_counter) {
    static int file_counter = 0;
    std::string file_name = "prime_twins_" + std::to_string(file_counter) + ".txt";
    std::ofstream file(file_name);
    for (size_t i = 0; i < prime_twins.size(); ++i) {
        file << (global_counter + 1) << ": " << prime_twins[i].first << ", " << prime_twins[i].second << std::endl;
        ++global_counter;
    }
    file.close();
    ++file_counter;
}

int main(int argc, char* argv[]) {
    int start = 3;
    int end = 1000000;
    int duration = 0;

    if (argc > 1) {
        start = std::stoi(argv[1]);
    }

    if (argc > 2) {
        end = std::stoi(argv[2]);
    }

    if (argc > 3) {
        duration = std::stoi(argv[3]);
    }

    std::vector<std::pair<int, int>> prime_twins;
    int global_counter = 0;

    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

    // Starte bei einer ungeraden Zahl
    if (start % 2 == 0) {
        ++start;
    }

    for (int i = start; i < end; i += 2) {
        if (is_prime(i) && is_prime(i + 2)) {
            prime_twins.emplace_back(i, i + 2);
            if (prime_twins.size() == 2000) {
                write_prime_twins_to_file(prime_twins, global_counter);
                prime_twins.clear();
            }
        }

        if (duration > 0 && std::chrono::steady_clock::now() >= start_time + std::chrono::seconds(duration)) {
            break;
        }
    }

    // Schreiben der verbleibenden Zwillinge, falls weniger als 2000 gefunden wurden
    if (!prime_twins.empty()) {
        write_prime_twins_to_file(prime_twins, global_counter);
    }

    return 0;
}
