#include "UniqueWordsCounter.h"

#include <chrono>
#include <iostream>

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [filename] \n";
        return 0;
    }

    std::ifstream istrm{argv[1], std::ios::binary};

    if (not istrm.is_open())
        throw std::runtime_error("Stream hasn't an associated file");

    if (not istrm.good())
        throw std::runtime_error("Error has occurred or I/O operations aren't available");

    const auto start = std::chrono::steady_clock::now();

    UniqueWordsCounter uniqueWordsCounter{istrm};

    const auto stop = std::chrono::steady_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

    std::cout << "[words: " << uniqueWordsCounter.all() << "] [unique: " << uniqueWordsCounter.unique() << "]\n";
    std::cout << "[time: " << static_cast<double>(time) / 1000000.0 << " s]\n";

    return 0;
}
