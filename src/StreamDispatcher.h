#pragma once

#include "IExecutionQueue.h"

#include <memory>

namespace core {
/// @brief ThreadPool-like object that provides context for task execution.
class IExecutionPool;
} // namespace core

class WordsCollection;
class WordsCounter;

class StreamDispatcher
{
public:
    StreamDispatcher(const std::string & filename, WordsCollection & wordsCollection);

    /// @brief Splits the file stream into parts approximately to the page value
    ///        and creates a words counter, run on the thread, for each range.
    void run();

private:
    /// @brief Expand the range of the origin value if it points to a character in a word.
    /// @return Origin value indicating the position of the character after the word.
    long expand(long origin);

    WordsCollection & m_wordsCollection;

    std::shared_ptr<core::IExecutionPool> m_executionPool;
    std::unique_ptr<core::IExecutionQueue<void(WordsCounter)>> m_executionQueue;
    std::unique_ptr<FILE, int (*)(FILE *)> m_fileStream;

    const long m_size;
    const long m_page;
};
