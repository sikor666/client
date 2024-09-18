#include <climits>
#include <fstream>
#include <iostream>
#include <set>
#include <thread>
#include <vector>

int main(int argc, char **argv)
{
    auto n = std::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";

    // seek to the end of stream immediately after open
    std::ifstream ifs{"data.txt", std::ios::binary | std::ios::ate};

    if (not ifs.is_open())
        throw std::runtime_error("Stream hasn't an associated file");

    if (not ifs.good())
        throw std::runtime_error("Error has occurred or I/O operations aren't available");

    auto fend = ifs.tellg();
    std::cout << "end: " << fend << "\n";

    ifs.seekg(0, std::ios_base::beg);

    auto fbeg = ifs.tellg();
    std::cout << "beg: " << fbeg << "\n";

    std::set<std::string> collection;

    while (not ifs.eof())
    {
        std::string word;
        ifs >> word;

        std::cout << word.size() << " " << (word.empty() ? "empty" : "full") << " " << word << "\n";

        if (not word.empty())
            collection.emplace(word);
    }

    // std::string content{std::istreambuf_iterator<char>{ifs}, std::istreambuf_iterator<char>{}};
    // std::cout << content.size() << " " << content << "\n";

    // std::string content0;
    // std::string content1;
    // ifs >> content0;
    // ifs >> content1;

    // std::cout << content0.size() << " " << content0 << "\n";
    // std::cout << content1.size() << " " << content1 << "\n";

    std::cout << "\n" << collection.size() <<" bye!\n";

    return 0;
}

/* std::string load(const std::string & filename)
{
  // Open the file
  std::FILE *fp = std::fopen(filename.c_str(), "rb");

  if (fp == nullptr) {
    throw std::runtime_error("IO_ERROR");
  }

  // Get the file size
  int ret;
  ret = std::fseek(fp, 0, SEEK_END);

  if(ret < 0) {
    std::fclose(fp);
    throw std::runtime_error("IO_ERROR");
  }

  long llen = std::ftell(fp);
  if((llen < 0) || (llen == LONG_MAX)) {
    std::fclose(fp);
    throw std::runtime_error("IO_ERROR");
  }

  // Allocate the padded_string
  size_t len = static_cast<size_t>(llen);
  std::vector<char> s(len);
  // padded_string s(len);
  // if (s.data() == nullptr) {
  //   std::fclose(fp);
  //   return MEMALLOC;
  // }

  // Read the padded_string
  std::rewind(fp);
  size_t bytes_read = std::fread(s.data(), 1, len, fp);
  if (std::fclose(fp) != 0 || bytes_read != len) {
    throw std::runtime_error("IO_ERROR");
  }

  return {s.begin(), s.end()};
}

int main(void)
{
    std::cout << load("data.txt");
} */

/* #include <iostream>
#include "simdjson.h"
using namespace simdjson;
int main(void) {
    ondemand::parser parser;
    padded_string json = padded_string::load("twitter.json");
    ondemand::document tweets = parser.iterate(json);
    std::cout << uint64_t(tweets["search_metadata"]["count"]) << " results." << std::endl;
} */