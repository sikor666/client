#include <fstream>
#include <iostream>
#include <string>
 
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [filename] \n";

        return 0;
    }

    std::string filename = argv[1];
    std::ofstream ostrm(filename, std::ios::trunc | std::ios::binary);

    if (not ostrm.is_open())
        throw std::runtime_error("Stream hasn't an associated file");

    if (not ostrm.good())
        throw std::runtime_error("Error has occurred or I/O operations aren't available");

    ostrm << "abc ";
    ostrm << "tomek ";
    ostrm << "jest ";
    ostrm << "pan";

    for (char i = 'a'; i <= 'z'; i++)
    {
        std::cout << i << " ";
    }

    return 0;
}
