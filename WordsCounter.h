#pragma once

#include <atomic>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

constexpr long PAGE_SIZE = 256 * 1024 * 1024;

class Words
{
public:
    Words()
        : numWords{0}
    {
    }

    void insert(std::unordered_set<std::string> & words)
    {
        // numWords += words.size();

        std::lock_guard<std::mutex> guard(mutexWords);
        uniqueWords.insert(words.begin(), words.end());
    }

    void print()
    {
        for (const auto & word : uniqueWords)
            std::cout << "[" << word << "]\n";
    }

    size_t number() const noexcept { return numWords.load(); }
    size_t unique() const noexcept { return uniqueWords.size(); }

    std::atomic<size_t> numWords;

private:
    std::mutex mutexWords;
    std::unordered_set<std::string> uniqueWords;
};

class Counter
{
public:
    Counter(Words & words, FILE * stream, long origin, long offset);

    void operator()();

private:
    Words & words;
    long offset;
    bool inWord;
    std::vector<char> buffer;
};
