#pragma once

#include <cstdio>
#include <vector>

class WordsCollection;

class WordsCounter
{
public:
    WordsCounter(WordsCollection & wordsCollection, FILE * fileStream, long streamOrigin, long streamOffset);

    void operator()();

private:
    WordsCollection & m_wordsCollection;
    long m_streamOffset;

    std::vector<char> m_streamBuffer;
};
