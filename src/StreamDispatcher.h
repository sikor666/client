#pragma once

#include "execq/IExecutionQueue.h"

#include <memory>

namespace execq {
/**
 * @class IExecutionPool
 * @brief ThreadPool-like object that provides context for task execution.
 */
class IExecutionPool;
} // namespace execq

class WordsCollection;
class WordsCounter;

class StreamDispatcher
{
public:
    StreamDispatcher(const std::string & filename, WordsCollection & wordsCollection);

    void run();

private:
    long find(long origin);

    WordsCollection & wordsCollection;

    std::shared_ptr<execq::IExecutionPool> pool;
    std::unique_ptr<execq::IExecutionQueue<void(WordsCounter)>> queue;
    std::unique_ptr<FILE, int (*)(FILE *)> stream;

    const long size;
    const long page;
};
