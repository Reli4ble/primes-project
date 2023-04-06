#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <gmpxx.h>
#include <regex>

mpz_class mod_pow(mpz_class base, mpz_class exp, mpz_class mod) {
    mpz_class result;
    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
    return result;
}

bool miller_rabin_test(mpz_class n, int k) {
    if (n < 2) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }

    mpz_class d = n - 1;
    while (d % 2 == 0) {
        d /= 2;
    }

    gmp_randclass rng(gmp_randinit_default);
    rng.seed(time(NULL));

    int n_bits = mpz_sizeinbase(n.get_mpz_t(), 2);

    for (int i = 0; i < k; ++i) {
        mpz_class a = rng.get_z_bits(n_bits - 1);
        mpz_class x = mod_pow(a, d, n);

        if (x == 1 || x == n - 1) {
            continue;
        }

        bool witness = true;
        for (int j = 1; j < n_bits; ++j) {
            x = mod_pow(x, 2, n);
            if (x == n - 1) {
                witness = false;
                break;
            }
        }

        if (witness) {
            return false;
        }
    }

    return true;
}

mpz_class parse_mersenne_number(const std::string &input) {
    mpz_class num;
    std::regex mersenne_regex(R"((\d+)\*2\^(\d+)-1)");
    std::smatch match;

    if (std::regex_match(input, match, mersenne_regex)) {
        mpz_class base = mpz_class(match[1].str());
        mpz_class exponent = mpz_class(match[2].str());

        mpz_class power;
        mpz_pow_ui(power.get_mpz_t(), mpz_class(2).get_mpz_t(), mpz_get_ui(exponent.get_mpz_t()));
        num = base * power - 1;
    } else {
        throw std::invalid_argument("Invalid Mersenne number");
    }
    return num;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <mersenne expression or file> <number_of_tests>" << std::endl;
        return 1;
    }

    int k = std::stoi(argv[2]);

    std::ifstream file(argv[1]);
    if (file) {
        std::string line;
        while (std::getline(file, line)) {
            try {
                mpz_class n = parse_mersenne_number(line);
                std::cout << n << " is " << (miller_rabin_test(n, k) ? "prime" : "composite") << "." << std::endl;
            } catch (const std::invalid_argument &e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    } else {
        try {
            mpz_class n = parse_mersenne_number(argv[1]);
                        std::cout << n << " is " << (miller_rabin_test(n, k) ? "prime" : "composite") << "." << std::endl;
        } catch (const std::invalid_argument &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
