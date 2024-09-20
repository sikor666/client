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
    WordsCollection & wordsCollection;
    long streamOffset;
    std::vector<char> streamBuffer;
};
