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
        {
            // When destroyed, queue waits until all tasks are executed
            StreamDispatcher streamDispatcher{argv[1], wordsCollection};
            streamDispatcher.run();
        }
        std::cout << wordsCollection.unique();
    }
    catch (const std::exception & ex)
    {
        std::cerr << ex.what() << '\n';
        return 1;
    }

    return 0;
}
