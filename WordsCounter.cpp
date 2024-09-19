#include "WordsCounter.h"

#include <vector>

constexpr auto BUFFER_SIZE = 256 * 1024;

WordsCounter::WordsCounter(std::ifstream & istrm)
    : inWord{false}
    , numWords{0}
{
    std::vector<char> buffer(BUFFER_SIZE + 1);
    std::string word;

    for (std::streamsize bytesRead; bytesRead = istrm.readsome(&buffer[0], BUFFER_SIZE);)
    {
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
                    uniqueWords.insert(word);
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
}
