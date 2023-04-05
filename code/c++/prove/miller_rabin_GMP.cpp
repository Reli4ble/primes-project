#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <gmpxx.h>

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

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <number or file> <number_of_tests>" << std::endl;
        return 1;
    }

    int k = std::stoi(argv[2]);

    mpz_class n;
    std::ifstream file(argv[1]);
    if (file) {
        while (file >> n) {
            std::cout << n << " is " << (miller_rabin_test(n, k) ? "prime" : "composite") << "." << std::endl;
        }
    } else {
        if (mpz_set_str(n.get_mpz_t(), argv[1], 10) == 0) {
            std::cout << n << " is " << (miller_rabin_test(n, k) ? "prime" : "composite") << "." << std::endl;
        } else {
            std::cerr << "Cannot parse number or open file: " << argv[1] << std::endl;
            return 1;
        }
    }

    return 0;
}
