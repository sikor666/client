#include "StreamDispatcher.h"
#include "WordsCollection.h"

#include <iostream>

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [filename] \n";
        return 0;
    }

    try
    {
        WordsCollection wordsCollection;
        // const auto start = std::chrono::steady_clock::now();
        {
            // When destroyed, queue waits until all tasks are executed
            StreamDispatcher streamDispatcher{argv[1], wordsCollection};
            streamDispatcher.run();
        }
        // const auto stop = std::chrono::steady_clock::now();
        // const auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

        // std::cout << "[time: " << static_cast<double>(time) / 1000000.0 << " s]\n";
        // std::cout << "[words: " << wordsCollection.number() << "] [unique: " << wordsCollection.unique() << "]\n";

        std::cout << wordsCollection.unique() << "\n";
    }
    catch (const std::exception & ex)
    {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
