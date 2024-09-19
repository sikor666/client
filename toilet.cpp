#include "UniqueWordsCounter.h"
#include "FileReader.h"

#include <chrono>
#include <fstream>
#include <iostream>

constexpr auto IO_BUFSIZE = 256 * 1024;

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   CURRENT_POS is the current file offset if known, negative if unknown.
   Return true if successful.  */
static void wc(std::ifstream & istrm, char const * file)
{
    char buf[IO_BUFSIZE + 1];
    long int words = 0;

    bool in_word = false;
    long int linepos = 0;

    for (std::streamsize bytes_read; bytes_read = istrm.readsome(buf, IO_BUFSIZE);)
    {
        char const * p = buf;
        do
        {
            unsigned char c = *p++;
            switch (c)
            {
                case '\r':
                case '\f':
                    linepos = 0;
                    in_word = false;
                    break;

                case '\t':
                    linepos += 8 - (linepos % 8);
                    in_word = false;
                    break;

                case ' ': linepos++;
                case '\v': in_word = false; break;

                default:
                    words += not in_word;
                    in_word = true;
                    break;
            }
        } while (--bytes_read);
    }

    std::cout << words << " " << file << "\n";
}

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [filename] \n";
        return 0;
    }

    // FileReader fileReader(argv[1]);
    // UniqueWordsCounter uniqueWordsCounter;

    std::ifstream istrm{argv[1], std::ios::binary};

    if (not istrm.is_open())
        throw std::runtime_error("Stream hasn't an associated file");

    if (not istrm.good())
        throw std::runtime_error("Error has occurred or I/O operations aren't available");

    const auto start = std::chrono::steady_clock::now();

    wc(istrm, argv[1]);

    const auto stop = std::chrono::steady_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

    std::cout << "time: " << static_cast<double>(time) / 1000000.0 << " s\n";

    return 0;
}
