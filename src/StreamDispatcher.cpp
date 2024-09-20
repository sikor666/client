#include "StreamDispatcher.h"
#include "WordsCounter.h"

#include <execq/execq.h>

#include <iostream>

StreamDispatcher::StreamDispatcher(const std::string & filename, WordsCollection & wordsCollection)
    : wordsCollection{wordsCollection}
    , pool{execq::CreateExecutionPool()}
    , queue{execq::CreateConcurrentExecutionQueue<void, WordsCounter>(pool,
          // Execution function is called in parallel on the next free thread with the next object from the queue
          [](const std::atomic_bool & isCanceled, WordsCounter && object) {
              object();
              std::cout << "done\n";
          })}
    , stream{std::unique_ptr<FILE, int (*)(FILE *)>{std::fopen(filename.c_str(), "rb"), std::fclose}}
    , size{std::fseek(stream.get(), 0, SEEK_END) == 0 ? std::ftell(stream.get()) : -1L}
    , page{64 * 1024 * 1024}
{
    if (size < 0)
        throw std::runtime_error("Stream hasn't an associated file");

    std::cout << "[size: " << size << "]\n";
}

void StreamDispatcher::run()
{
    for (long origin = 0, offset = 0; origin < size; origin += offset)
    {
        offset = find(origin + std::min(page, size - origin)) - origin;
        std::cout << "[origin: " << origin << "] [offset: " << offset << "]\n";

        queue->push(WordsCounter{wordsCollection, stream.get(), origin, offset});
    }
}

long StreamDispatcher::find(long origin)
{
    std::fseek(stream.get(), origin, SEEK_SET);

    for (int c; (c = std::fgetc(stream.get())) != EOF;)
    {
        if (c == ' ' or c == '\t' or c == '\n')
            break;

        std::fseek(stream.get(), ++origin, SEEK_SET);
    }

    return std::ftell(stream.get());
}
