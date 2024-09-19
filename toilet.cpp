#include "WordsCounter.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [filename] \n";
        return 0;
    }

    auto n = std::thread::hardware_concurrency();
    std::cout << "[threads: " << n << "]\n";

    // std::ifstream istrm{argv[1], std::ios::binary};
    // if (not istrm.is_open())
    //     throw std::runtime_error("Stream hasn't an associated file");
    // if (not istrm.good())
    //     throw std::runtime_error("Error has occurred or I/O operations aren't available");

    const auto file = std::unique_ptr<FILE, int (*)(FILE *)>{std::fopen(argv[1], "rb"), std::fclose};
    const auto start = std::chrono::steady_clock::now();

    // WordsCounter wordsCounter{istrm};
    WordsCounter wordsCounter{file.get()};

    const auto stop = std::chrono::steady_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

    std::cout << "[words: " << wordsCounter.number() << "] [unique: " << wordsCounter.unique() << "]\n";
    std::cout << "[time: " << static_cast<double>(time) / 1000000.0 << " s]\n";

    return 0;
}
