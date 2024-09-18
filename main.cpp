#include <climits>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

std::string load(const std::string & filename)
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
}

/* int main(int argc, char **argv)
{
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";

    std::ifstream ifs{"data.txt"};

    ifs.seekg(0, std::ios_base::end);

    std::cout << ifs.tellg() << "\n";

    ifs.seekg(0, std::ios_base::beg);

    std::cout << ifs.tellg() << "\n";

    std::string content;
    content.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

    std::cout << content.size() << " " << content << "\n";

    std::cout << (content[0] == '\n' ? "NL" : "DP") << "\n";

    return 0;
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