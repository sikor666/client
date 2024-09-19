#pragma once

#include <fstream>
#include <unordered_set>

class UniqueWordsCounter
{
public:
    UniqueWordsCounter(std::ifstream & istrm);

    size_t all() const noexcept { return numWords; }
    size_t unique() const noexcept { return uniqueWords.size(); }

private:
    bool inWord;
    size_t numWords;

    std::unordered_set<std::string> uniqueWords;
};
