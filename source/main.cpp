#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

namespace custom {

template<typename InputIterator, typename OutputIterator, typename Comparator>
void merge(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2, OutputIterator out, Comparator predicate) {
    while (first1 != last1 && first2 != last2) {
        if (predicate(*first1, *first2)) {
            *out = *first2;
            first2 = std::next(first2);
        } else {
            *out = *first1;
            first1 = std::next(first1);
        }
        out++;
    }
    // Handle tail now
    auto [tail_forwarder, tail_end] = first1 < last1 ? std::make_pair(first1, last1) : std::make_pair(first2, last2);
    while (tail_forwarder != tail_end) {
        *out = *tail_forwarder;
        tail_forwarder = std::next(tail_forwarder);
    }
}

template<typename InputIterator, typename OutputIterator>
void merge(InputIterator first1, InputIterator last1, InputIterator first2, InputIterator last2, OutputIterator out) {
    merge(std::forward<InputIterator>(first1), std::forward<InputIterator>(last1),
          std::forward<InputIterator>(first2), std::forward<InputIterator>(last2),
          std::forward<OutputIterator>(out), std::greater<typename InputIterator::value_type>());
}

}


int main(int argc, char *argv[]) {
    std::vector<int> part1 = {10, 23, 4, 2, 14};
    std::vector<int> part2 = {59, 74, 23, 12};
    std::vector<int> merger;
    custom::merge(part1.begin(), part1.end(), part2.begin(), part2.end(), std::back_inserter(merger));

    //    std::filesystem::path resources = "resources/";
    //
    //    std::string source_filename = "data.txt";
    //    std::filesystem::path source = resources.string() + source_filename;
    //
    //    std::fstream file(source.c_str(), std::ios::in);
    //    if (!file.is_open()) {
    //        std::cerr << "Cannot open file for reading data" << std::endl;
    //        return 1;
    //    }
    //    std::istream_iterator<int> source_fwd(file);
    //    std::istream_iterator<int> source_end;
    //
    //    std::vector<int> buffer;
    //
    //    std::cout << "Start copy" << std::endl;
    //    const auto start_reading_time = std::chrono::system_clock::now().time_since_epoch();
    //    std::copy(source_fwd, source_end, std::back_inserter(buffer));
    //    const auto end_reading_time = std::chrono::system_clock::now().time_since_epoch();
    //    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds>(end_reading_time - start_reading_time).count() << std::endl;
    //    std::cout << "Done" << std::endl;
    //
    //    std::cout << "Start sorting" << std::endl;
    //    const auto start_sorting_time = std::chrono::system_clock::now().time_since_epoch();
    //    std::sort(buffer.begin(), buffer.end());
    //    const auto end_sorting_time = std::chrono::system_clock::now().time_since_epoch();
    //    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds>(end_sorting_time - start_sorting_time).count() << std::endl;
    //    std::cout << "Done" << std::endl;
    //
    //    std::string out_filename = "out.txt";
    //    std::filesystem::path out = resources.string() + out_filename;
    //    std::fstream out_file(out, std::ios::out | std::ios::trunc);
    //    if (!out_file.is_open()) {
    //        std::cerr << "Cannot open file for writing sorted data" << std::endl;
    //        return 1;
    //    }
    //
    //    std::cout << "Start saving" << std::endl;
    //    const auto start_writing_time = std::chrono::system_clock::now().time_since_epoch();
    //    std::ostream_iterator<int> out_fwd(out_file, " ");
    //    std::copy(buffer.begin(), buffer.end(), out_fwd);
    //    const auto end_writing_time = std::chrono::system_clock::now().time_since_epoch();
    //    std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::seconds>(end_writing_time - start_writing_time).count() << std::endl;
    //    std::cout << "Done" << std::endl;

    return 0;
}