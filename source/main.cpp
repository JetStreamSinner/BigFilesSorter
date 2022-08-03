#include "custom.h"
#include <filesystem>


int main(int argc, char *argv[]) {
    const std::filesystem::path path = "resources/data.txt";
    const std::size_t bytes_restriction = 10;
    custom::external_sort(path, bytes_restriction);
    return 0;
}