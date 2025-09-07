#include "utils.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;
namespace utils
{

bool fileExists(const std::string &file)
{
    return std::ifstream(file).good();
}


}
