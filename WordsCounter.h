#pragma once

#include <fstream>
#include <unordered_set>

class WordsCounter
{
public:
    WordsCounter(std::ifstream & istrm);
    WordsCounter(FILE * stream, long origin, long offset);

    size_t number() const noexcept { return numWords; }
    size_t unique() const noexcept { return uniqueWords.size(); }

private:
    bool inWord;
    size_t numWords;

    std::unordered_set<std::string> uniqueWords;
};
