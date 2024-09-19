#include "FileReader.h"

#include <stdexcept>

FileReader::FileReader(const std::string & filename)
    : file{std::fopen(filename.c_str(), "rb")}
{
    if (file == nullptr)
        throw std::runtime_error("File open error");
}

FileReader::~FileReader()
{
    std::fclose(file);

    // FIXME:
    // if (std::fclose(file) != 0)
    //     throw std::runtime_error("File close error");
}

size_t FileReader::read()
{
    // bytes_read = std::fread(buf, 1, IO_BUFSIZE, fp));

    return 0;
}
