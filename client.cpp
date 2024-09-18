#include <climits>
#include <fstream>
#include <iostream>
#include <set>
#include <thread>
#include <vector>

#include <experimental/simd>

namespace stdx = std::experimental;

using floatv  = stdx::native_simd<float>;
using doublev = stdx::rebind_simd_t<double, floatv>;
using intv    = stdx::rebind_simd_t<int, floatv>;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [filename] \n";

        return 0;
    }

    std::cout << argc << "\n";

    while (argc > 0)
    {
        std::cout << argv[--argc] << "\n";
    }

    auto n = std::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";

    // seek to the end of stream immediately after open
    std::ifstream ifs{"data.txt", std::ios::binary | std::ios::ate};

    if (not ifs.is_open())
        throw std::runtime_error("Stream hasn't an associated file");

    if (not ifs.good())
        throw std::runtime_error("Error has occurred or I/O operations aren't available");

    auto fend = ifs.tellg();
    std::cout << "end: " << fend << "\n";

    ifs.seekg(0, std::ios_base::beg);

    auto fbeg = ifs.tellg();
    std::cout << "beg: " << fbeg << "\n";

    std::set<std::string> collection;

    const auto start = std::chrono::steady_clock::now();

    while (not ifs.eof())
    {
        std::string word;
        ifs >> word;

        std::cout << word.size() << " " << (word.empty() ? "empty" : "full") << " " << word << "\n";

        if (not word.empty())
            collection.emplace(word);
    }

    const auto stop = std::chrono::steady_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

    std::cout << "\n" << collection.size() << " time: " << time << " us\n";

    return 0;
}
