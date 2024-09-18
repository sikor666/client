#include <ctype.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>

constexpr auto IO_BUFSIZE = 256 * 1024;

static bool wc_isspace[UCHAR_MAX + 1];

/* Count words.  FILE_X is the name of the file (or null for standard
   input) that is open on descriptor FD.  *FSTATUS is its status.
   CURRENT_POS is the current file offset if known, negative if unknown.
   Return true if successful.  */
static bool wc(int fd, char const * file_x, off_t current_pos)
{
    int err = 0;
    char buf[IO_BUFSIZE + 1];
    long int words = 0;

    bool in_word = false;
    long int linepos = 0;

    for (ssize_t bytes_read; (bytes_read = read(fd, buf, IO_BUFSIZE));)
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
                    bool in_word2 = !wc_isspace[c];
                    words += !in_word & in_word2;
                    in_word = in_word2;
                    break;
            }
        } while (--bytes_read);
    }

    printf("%ld\n %s\n", words, file_x);

    if (err)
        error(0, err, "%s", file_x);
    return !err;
}

static bool wc_file(char const * file)
{
    int fd = open(file, O_RDONLY);
    if (fd == -1)
    {
        error(0, errno, "%s", file);
        return false;
    }
    else
    {
        bool ok = wc(fd, file, 0);
        if (close(fd) != 0)
        {
            error(0, errno, "%s", file);
            return false;
        }
        return ok;
    }
}

int main(int argc, char ** argv)
{
    for (int i = 0; i <= UCHAR_MAX; i++)
        wc_isspace[i] = isspace(i);

    bool ok = true;

    ok &= wc_file(argv[1]);

    return ok ? 0 : 1;
}
