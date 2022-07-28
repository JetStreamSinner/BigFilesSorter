#include <iostream>
#include <filesystem>
#include <chrono>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>

int main(int argc, char *argv[]) {
    std::filesystem::path resources = "resources/";

    std::string source_filename = "data.txt";
    std::filesystem::path source = resources.string() + source_filename;

    std::fstream file(source.c_str(), std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Cannot open file for reading data" << std::endl;
        return 1;
    }
    std::istream_iterator<int> source_fwd(file);
    std::istream_iterator<int> source_end;

    std::vector<int> buffer;

    std::cout << "Start copy" << std::endl;
    const auto start_reading_time = std::chrono::system_clock::now().time_since_epoch();
    std::copy(source_fwd, source_end, std::back_inserter(buffer));
    const auto end_reading_time = std::chrono::system_clock::now().time_since_epoch();
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds>(end_reading_time - start_reading_time).count() << std::endl;
    std::cout << "Done" << std::endl;

    std::cout << "Start sorting" << std::endl;
    const auto start_sorting_time = std::chrono::system_clock::now().time_since_epoch();
    std::sort(buffer.begin(), buffer.end());
    const auto end_sorting_time = std::chrono::system_clock::now().time_since_epoch();
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds>(end_sorting_time - start_sorting_time).count() << std::endl;
    std::cout << "Done" << std::endl;

    std::string out_filename = "out.txt";
    std::filesystem::path out = resources.string() + out_filename;
    std::fstream out_file(out, std::ios::out | std::ios::trunc);
    if (!out_file.is_open()) {
        std::cerr << "Cannot open file for writing sorted data" << std::endl;
        return 1;
    }

    std::cout << "Start saving" << std::endl;
    const auto start_writing_time = std::chrono::system_clock::now().time_since_epoch();
    std::ostream_iterator<int> out_fwd(out_file, " ");
    std::copy(buffer.begin(), buffer.end(), out_fwd);
    const auto end_writing_time = std::chrono::system_clock::now().time_since_epoch();
    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds>(end_writing_time - start_writing_time).count() << std::endl;
    std::cout << "Done" << std::endl;

    return 0;
}