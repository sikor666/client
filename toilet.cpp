#include "UniqueWordsCounter.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

constexpr auto IO_BUFSIZE = 256 * 1024;

/// Count words
static void wc(std::ifstream & istrm, char const * file)
{
    size_t words = 0;
    bool inWord = false;

    std::vector<char> buf(IO_BUFSIZE + 1);
    std::string str;
    std::unordered_set<std::string> cache;

    for (std::streamsize bytesRead; bytesRead = istrm.readsome(&buf[0], IO_BUFSIZE);)
    {
        const auto * ptr = buf.data();
        do
        {
            unsigned char c = *ptr++;
            switch (c)
            {
                case '\n':
                case ' ':
                    inWord = false;
                    // std::cout << str.size() << " " << str << "\n";
                    cache.insert(str);
                    str.clear();
                    break;

                default:
                    words += not inWord;
                    inWord = true;
                    str += c;
                    break;
            }
        } while (--bytesRead);
    }

    std::cout << "[words: " << words << "] [unique: " << cache.size() << "] [file: " << file << "]\n";
}

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [filename] \n";
        return 0;
    }

    // UniqueWordsCounter uniqueWordsCounter;

    std::ifstream istrm{argv[1], std::ios::binary};

    if (not istrm.is_open())
        throw std::runtime_error("Stream hasn't an associated file");

    if (not istrm.good())
        throw std::runtime_error("Error has occurred or I/O operations aren't available");

    const auto start = std::chrono::steady_clock::now();

    wc(istrm, argv[1]);

    const auto stop = std::chrono::steady_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

    std::cout << "[time: " << static_cast<double>(time) / 1000000.0 << " s]\n";

    return 0;
}
