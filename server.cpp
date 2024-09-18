#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include <experimental/random>

constexpr auto MAX_WORDS = 32000000ul;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [filename] \n";

        return 0;
    }

    std::string filename = argv[1];
    std::ofstream ostrm(filename, std::ios::trunc | std::ios::binary);

    if (not ostrm.is_open())
        throw std::runtime_error("Stream hasn't an associated file");

    if (not ostrm.good())
        throw std::runtime_error("Error has occurred or I/O operations aren't available");

    const auto start = std::chrono::steady_clock::now();

    for (size_t i = 0; i < MAX_WORDS; i++)
    {
        std::cout << "\r" << (static_cast<double>(i) / static_cast<double>(MAX_WORDS)) * 100.0 << " %";

        size_t len = std::experimental::randint(1, 16);

        while (len-- > 0)
        {
            ostrm << static_cast<char>(std::experimental::randint(int('a'), int('z')));
        }

        ostrm << " ";
    }

    const auto stop = std::chrono::steady_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::seconds>(stop - start).count();

    std::cout << "\n" << "time: " << time << " s\n";

    return 0;
}
