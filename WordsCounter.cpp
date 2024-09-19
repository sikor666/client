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

WordsCounter::WordsCounter(FILE * stream, long origin, long offset)
    : inWord{false}
    , numWords{0}
{
    std::vector<char> buffer(BUFFER_SIZE + 1);
    std::string word;
    std::streamsize bytes = 0;

    // std::fseek(stream, 16, SEEK_SET);
    std::fseek(stream, 0, SEEK_END); // seek to end
    std::cout << "[size: " << std::ftell(stream) << "]\n";
    std::fseek(stream, 0, SEEK_SET); // seek to start

    for (std::streamsize bytesRead; bytesRead = std::fread(&buffer[0], 1, BUFFER_SIZE, stream);)
    {
        bytes += bytesRead;

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

    std::cout << "[bytes: " << bytes << "]\n";
}
