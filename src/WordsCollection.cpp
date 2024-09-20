#include "WordsCollection.h"

WordsCollection::WordsCollection()
    : numWords{0}
{
}

size_t WordsCollection::operator+=(size_t number)
{
    return numWords += number;
}

void WordsCollection::insert(std::unordered_set<std::string> & words)
{
    std::lock_guard<std::mutex> guard(mutexWords);
    uniqueWords.insert(words.begin(), words.end());
}

size_t WordsCollection::number() const noexcept
{
    return numWords.load();
}

size_t WordsCollection::unique() const noexcept
{
    return uniqueWords.size();
}
