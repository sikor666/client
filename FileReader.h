#pragma once

#include <string>

class FileReader
{
public:
    FileReader(const std::string & filename);
    ~FileReader();

    size_t read();

private:
    FILE * file;
};
