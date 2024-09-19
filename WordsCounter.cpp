#include "WordsCounter.h"

#include <iostream>
#include <vector>

constexpr auto BUFFER_SIZE = 256 * 1024;

WordsCounter::WordsCounter(std::ifstream & istrm)
    : inWord{false}
    , numWords{0}
{
    std::vector<char> buffer(BUFFER_SIZE + 1);
    std::string word;
    std::streamsize bytes = 0;

    for (std::streamsize bytesRead; bytesRead = istrm.readsome(&buffer[0], BUFFER_SIZE);)
    {
        bytes += bytesRead;

        // std::cout << bytesRead << "\n"; 35289717
        // 3200000000
        // 3200000000
        // 32000000

        const auto * ptr = buffer.data();
        do
        {
            unsigned char c = *ptr++;
            switch (c)
            {
                case '\n':
                case ' ':
                    inWord = false;
                    // std::cout << word.size() << " " << word << "\n";
                    // uniqueWords.insert(word);
                    word.clear();
                    break;

                default:
                    numWords += not inWord;
                    inWord = true;
                    word += c;
                    break;
            }
        } while (--bytesRead);
    }

    std::cout << "[bytes: " << bytes << "]\n";
}