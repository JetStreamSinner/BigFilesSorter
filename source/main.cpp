#include <iostream>
#include <algorithm>
#include <filesystem>
#include <fstream>
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

template<typename It>
std::vector<std::size_t> get_chunk(It& forwarder, std::size_t bytes_in_chunk) {
    std::vector<std::size_t> chunk_data;
    It end;
    for (std::size_t bytes_read = 0; forwarder != end && bytes_read < bytes_in_chunk;) {
        chunk_data.push_back(*forwarder);
        const auto delim_bytes = 1;
        bytes_read += std::to_string(*forwarder).size() + delim_bytes;
        forwarder = std::next(forwarder);
    }
    return std::move(chunk_data);
}

void split_source_in_chunks(const std::filesystem::path& path, std::size_t bytes_in_chunk) {

    if (bytes_in_chunk <= 0) {
        throw std::runtime_error("Bad chunk size");
    }

    std::fstream file(path, std::fstream::in);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open source file");
    }

    const std::filesystem::path temp_directory = "tmp/";
    std::filesystem::create_directory(temp_directory);

    std::istream_iterator<std::size_t> forwarder(file);
    std::istream_iterator<std::size_t> end;
    for (std::size_t chunk_index = 0;forwarder != end; ++chunk_index) {
        auto next_chunk = get_chunk(forwarder, bytes_in_chunk);
        const std::filesystem::path chunk_file_name = temp_directory.string() + std::to_string(chunk_index);
        std::fstream chunk_file(chunk_file_name, std::fstream::out);
        std::ostream_iterator<std::size_t> chunk_file_iterator(chunk_file, " ");
        std::move(next_chunk.begin(), next_chunk.end(), chunk_file_iterator);
    }

    std::filesystem::remove_all(temp_directory);
}

}


int main(int argc, char *argv[]) {
    custom::split_source_in_chunks("resources/data.txt", 1);
    return 0;
}