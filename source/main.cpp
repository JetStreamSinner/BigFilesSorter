#include "custom.h"
#include <filesystem>


int main(int argc, char *argv[]) {
    //    const std::filesystem::path path = "resources/data.txt";
    //    const std::size_t bytes_restriction = 10;
    //    custom::external_sort(path, bytes_restriction);

    std::vector<std::size_t> first = {1, 2, 3};
    std::vector<std::size_t> second = {4, 5, 6};
    std::vector<std::size_t> third = {7, 8, 9};
    std::vector<std::size_t> fourth = {10, 11, 12};
    using Iterator = std::vector<std::size_t>::iterator;
    std::vector<custom::Range<Iterator>> target = {{second.begin(), second.end()},
                                                   {first.begin(), first.end()},
                                                   {third.begin(), third.end()},
                                                   {fourth.begin(), fourth.end()}};
    std::vector<std::size_t> merged;
    custom::merge_n(target, std::back_inserter(merged));
    return 0;
}