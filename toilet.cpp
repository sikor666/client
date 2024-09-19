#include "WordsCounter.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include <execq/execq.h>

// The function is called in parallel on the next free thread
// with the next object from the queue.
void ProcessObject(const std::atomic_bool & isCanceled, Counter && object)
{
    // if (isCanceled)
    // {
    //     std::cout << "Queue has been canceled. Skipping object...";
    //     return;
    // }

    // std::cout << "Processing object: " << object << '\n';
    object();
}

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [filename] \n";
        return 0;
    }

    const auto n = std::thread::hardware_concurrency();
    std::cout << "[threads: " << n << "]\n";

    const auto stream = std::unique_ptr<FILE, int (*)(FILE *)>{std::fopen(argv[1], "rb"), std::fclose};
    std::fseek(stream.get(), 0, SEEK_END);

    const auto size = std::ftell(stream.get());
    std::cout << "[size: " << size << "]\n";

    const auto find = [&stream](long origin) {
        std::fseek(stream.get(), origin, SEEK_SET);

        for (int c; (c = std::fgetc(stream.get())) != EOF;)
        {
            if (c == ' ' or c == '\t' or c == '\n')
                break;

            std::fseek(stream.get(), ++origin, SEEK_SET);
        }

        return std::ftell(stream.get());
    };

    Words words;

    const auto start = std::chrono::steady_clock::now();

    try
    {
        std::shared_ptr<execq::IExecutionPool> pool = execq::CreateExecutionPool();
        std::unique_ptr<execq::IExecutionQueue<void(Counter)>> queue =
            execq::CreateConcurrentExecutionQueue<void, Counter>(pool, &ProcessObject);

        for (long origin = 0, offset = 0; origin < size; origin += offset)
        {
            offset = find(origin + std::min(PAGE_SIZE, size - origin)) - origin;
            std::cout << "\n[origin: " << origin << "] [offset: " << offset << "]\n";

            queue->push(Counter{words, stream.get(), origin, offset});

            // Counter{words, stream.get(), origin, offset}();
        }

        // when destroyed, queue waits until all tasks are executed
    }
    catch (const std::exception & e)
    {
        std::cerr << e.what() << '\n';
    }

    std::cout << "Wakeup\n";

    const auto stop = std::chrono::steady_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

    std::cout << "\n[time: " << static_cast<double>(time) / 1000000.0 << " s]\n";
    std::cout << "[words: " << words.number() << "] [unique: " << words.unique() << "]\n";

    // words.print();

    return 0;
}
