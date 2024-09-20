#include "WordsCollection.h"

WordsCollection::WordsCollection()
    : m_numWords{0}
{
}

size_t WordsCollection::operator+=(size_t number)
{
    return m_numWords += number;
}

void WordsCollection::insert(std::unordered_set<std::string> & words)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_uniqueWords.insert(words.begin(), words.end());
}

size_t WordsCollection::number() const noexcept
{
    return m_numWords.load();
}

size_t WordsCollection::unique() const noexcept
{
    return m_uniqueWords.size();
}
