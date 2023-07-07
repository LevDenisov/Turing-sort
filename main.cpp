#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "Tape.h"
#include "interface.h"

boost::property_tree::ptree config;

// A function to return a seeded random number generator.
inline std::mt19937 &generator() {
    // the generator will only be seeded once (per thread) since it's static
    static thread_local std::mt19937 gen(std::random_device{}());
    return gen;
}

// A function to generate integers in the range [min, max]
template <typename T, std::enable_if_t<std::is_integral_v<T>> * = nullptr>
T my_rand(T min, T max) {
    std::uniform_int_distribution<T> dist(min, max);
    return dist(generator());
}

template <typename T> void rand_value(std::vector<T> &arr) {
    for (auto &v : arr) {
        v = my_rand(0, 1000);
    }
}

bool isEqualSorted(std::vector<T> arr, Tape &tape) {
    tape.headBegin();

    std::vector<T> arr2(tape.count());

    for (uint i = 0; i < tape.count(); i++, tape.moveRight()) {
        arr2[i] = tape.getData();
    }

    std::sort(arr.begin(), arr.end());
    std::sort(arr2.begin(), arr2.end());

    if (arr.size() != arr2.size()) {
        return false;
    }

    for (uint i = 0; i < arr.size(); i++) {
        if (arr[i] != arr2[i]) {
            return false;
        }
    }

    return true;
}

int main() {
    boost::property_tree::ini_parser::read_ini("config.ini", config);

    const uint sizeArr = 2 * 1024;
    const uint N_RUNS = 10;

    const std::string path_in = "input.txt";
    const std::string path_out = "output.txt";
    const uint RAM = 128;

    Interface interface(RAM);

    std::vector<T> arr(sizeArr);

    for (uint t = 0; t < N_RUNS; t++) {
        std::cout << "Test #" << t + 1 << " ";

        rand_value(arr);

        std::ofstream file("input.txt", std::ios::binary | std::ios::trunc);

        file.write(reinterpret_cast<char *>(arr.data()), sizeArr * sizeof(T));
        file.close();

        interface.Extern_sort(path_in, path_out);

        Tape out("output.txt");

        if (interface.isSorted(out) && isEqualSorted(arr, out)) {
            std::cout << "COMPLETE" << std::endl;
        } else {
            std::cout << "FAILED!!!" << std::endl;
            return 1;
        }
    }
    return 0;
}