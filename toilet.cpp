#include "UniqueWordsCounter.h"
#include "FileReader.h"

#include <chrono>
#include <iostream>

constexpr auto IO_BUFSIZE = 256 * 1024;

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   CURRENT_POS is the current file offset if known, negative if unknown.
   Return true if successful.  */
static void wc(std::FILE * fp, char const * file)
{
    int err = 0;
    char buf[IO_BUFSIZE + 1];
    long int words = 0;

    bool in_word = false;
    long int linepos = 0;

    for (size_t bytes_read; (bytes_read = std::fread(buf, 1, IO_BUFSIZE, fp));)
    {
        if (bytes_read < 0)
        {
            err = errno;
            break;
        }

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

    if (err)
        throw std::runtime_error("File read error");
}

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [filename] \n";
        return 0;
    }

    char const * file = argv[1];

    auto * fp = std::fopen(file, "rb");
    if (fp == nullptr)
        throw std::runtime_error("File open error");

    FileReader fileReader(argv[1]);
    UniqueWordsCounter uniqueWordsCounter;

    const auto start = std::chrono::steady_clock::now();

    wc(fp, file);

    const auto stop = std::chrono::steady_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();

    std::cout << "time: " << static_cast<double>(time) / 1000000.0 << " s\n";

    if (std::fclose(fp) != 0)
        throw std::runtime_error("File close error");

    return 0;
}
