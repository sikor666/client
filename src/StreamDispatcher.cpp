#include "StreamDispatcher.h"
#include "ExecutionFactory.h"
#include "WordsCounter.h"

#include <iostream>

StreamDispatcher::StreamDispatcher(const std::string & filename, WordsCollection & wordsCollection)
    : m_wordsCollection{wordsCollection}
    , m_executionPool{core::CreateExecutionPool()}
    , m_executionQueue{core::CreateConcurrentExecutionQueue<void, WordsCounter>(m_executionPool,
          // Execution function is called in parallel on the next free thread with the next object from the queue
          [](const std::atomic_bool & isCanceled, WordsCounter && object) {
              object();
              std::cout << "done\n";
          })}
    , m_fileStream{std::unique_ptr<FILE, int (*)(FILE *)>{std::fopen(filename.c_str(), "rb"), std::fclose}}
    , m_size{std::fseek(m_fileStream.get(), 0, SEEK_END) == 0 ? std::ftell(m_fileStream.get()) : -1L}
    , m_page{64 * 1024 * 1024}
{
    if (m_size < 0)
        throw std::runtime_error("Stream hasn't an associated file");

    std::cout << "[size: " << m_size << "]\n";
}

void StreamDispatcher::run()
{
    for (long origin = 0, offset = 0; origin < m_size; origin += offset)
    {
        offset = expand(origin + std::min(m_page, m_size - origin)) - origin;
        std::cout << "[origin: " << origin << "] [offset: " << offset << "]\n";

        m_executionQueue->push(WordsCounter{m_wordsCollection, m_fileStream.get(), origin, offset});
    }
}

long StreamDispatcher::expand(long origin)
{
    std::fseek(m_fileStream.get(), origin, SEEK_SET);

    for (int c; (c = std::fgetc(m_fileStream.get())) != EOF;)
    {
        if (c == ' ' or c == '\t' or c == '\n')
            break;

        std::fseek(m_fileStream.get(), ++origin, SEEK_SET);
    }

    return std::ftell(m_fileStream.get());
}
