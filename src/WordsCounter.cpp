#include "WordsCounter.h"
#include "WordsCollection.h"

#include <unordered_set>

WordsCounter::WordsCounter(WordsCollection & wordsCollection, FILE * fileStream, long streamOrigin, long streamOffset)
    : wordsCollection{wordsCollection}
    , streamOffset{streamOffset}
{
    streamBuffer.resize(streamOffset + 1);

    std::fseek(fileStream, streamOrigin, SEEK_SET);
    std::fread(&streamBuffer[0], 1, streamOffset, fileStream);
}

void WordsCounter::operator()()
{
    bool inWord = false;
    size_t wordsNumber = 0;

    std::string word;
    std::unordered_set<std::string> words;

    const auto * ptr = &streamBuffer[0];
    do
    {
        const auto chr = *ptr++;
        switch (chr)
        {
            case '\t':
            case '\n':
            case ' ':
                if (not word.empty())
                    words.insert(word);

                inWord = false;
                word.clear();
                break;

            default:
                wordsNumber += not inWord;

                inWord = true;
                word += chr;
                break;
        }
    } while (--streamOffset);

    wordsCollection += wordsNumber;
    wordsCollection.insert(words);
}
