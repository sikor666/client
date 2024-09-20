#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <unordered_set>

class WordsCollection
{
public:
    WordsCollection();

    size_t operator+=(size_t number);

    void insert(std::unordered_set<std::string> & words);

    size_t number() const noexcept;
    size_t unique() const noexcept;

private:
    std::mutex mutexWords;
    std::atomic<size_t> numWords;
    std::unordered_set<std::string> uniqueWords;
};
