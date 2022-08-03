#include "custom.h"
#include <filesystem>


int main(int argc, char *argv[]) {
    //    const std::filesystem::path path = "resources/data.txt";
    //    const std::size_t bytes_restriction = 10;
    //    custom::external_sort(path, bytes_restriction);

    std::vector<std::size_t> third = {70, 85, 912};
    std::vector<std::size_t> fourth = {124, 11543, 12330};
    std::vector<std::size_t> second = {std::numeric_limits<std::size_t>::min(), 4123, 5453};
    std::vector<std::size_t> first = {0, std::numeric_limits<std::size_t>::min(), 6};
    using Iterator = std::vector<std::size_t>::iterator;
    std::vector<custom::Range<Iterator>> target = {{second.begin(), second.end()},
                                                   {first.begin(), first.end()},
                                                   {third.begin(), third.end()},
                                                   {fourth.begin(), fourth.end()}};
    std::vector<std::size_t> merged;
    custom::merge_n(target, std::back_inserter(merged));
    return 0;
}