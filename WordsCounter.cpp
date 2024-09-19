#include "WordsCounter.h"

#include <iostream>

Counter::Counter(Words & words, FILE * stream, long origin, long offset)
    : words{words}
    , offset{offset}
    , inWord{false}
{
    buffer.resize(offset + 1);

    std::fseek(stream, origin, SEEK_SET);
    // std::fread(&buffer[0], 1, offset, stream);

    std::cout << "[bytes: " << std::fread(&buffer[0], 1, offset, stream) << "] ";
}

void Counter::operator()()
{
    size_t number = 0;
    std::string word;
    std::unordered_set<std::string> data;

    const auto * ptr = &buffer[0];
    do
    {
        unsigned char c = *ptr++;
        switch (c)
        {
            case '\t':
            case '\n':
            case ' ':
                inWord = false;
                if (not word.empty())
                    data.insert(word);
                word.clear();
                break;

            default:
                number += not inWord;
                inWord = true;
                word += c;
                break;
        }
    } while (--offset);

    words.numWords += number;
    words.insert(data);

    std::cout << "Done\n";
}

// text.txt
// 6  6  6  _  H  a  c  k  e  r  _  | T  o  m  e  k  _  6  6  6  _  | H  a  c  k  e  r  _  T  o  m  e  k  \n
// 00 01 02 03 04 05 06 07 08 09 10 | 11 12 13 14 15 16 17 18 19 20 | 21 22 23 24 25 26 27 28 29 30 31 32 33
