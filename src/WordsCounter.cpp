#include "WordsCounter.h"
#include "WordsCollection.h"

#include <stdexcept>
#include <unordered_set>

WordsCounter::WordsCounter(WordsCollection & wordsCollection, FILE * fileStream, long streamOrigin, long streamOffset)
    : m_wordsCollection{wordsCollection}
    , m_streamOffset{streamOffset}
{
    m_streamBuffer.resize(streamOffset + 1);

    if (std::fseek(fileStream, streamOrigin, SEEK_SET) != 0)
        throw std::runtime_error("Cannot set file position indicator for file stream");

    for (size_t bytesRead = 0;
         (bytesRead += std::fread(&m_streamBuffer[bytesRead], 1, streamOffset, fileStream)) < streamOffset;)
        ;
}

void WordsCounter::operator()()
{
    bool inWord = false;
    size_t wordsNumber = 0;

    std::string word;
    std::unordered_set<std::string> words;

    const auto * ptr = &m_streamBuffer[0];
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
    } while (--m_streamOffset);

    m_wordsCollection += wordsNumber;
    m_wordsCollection.insert(words);
}
