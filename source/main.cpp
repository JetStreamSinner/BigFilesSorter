#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

namespace custom {

    struct ChunksInfo {
        std::string chunk_name;
        template<typename T>
        explicit ChunksInfo(const T &chunk_name) : chunk_name(chunk_name) {
        }
    };

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
    std::vector<std::size_t> get_chunk(It &forwarder, std::size_t bytes_in_chunk) {
        std::vector<std::size_t> chunk_data;
        It end;
        for (std::size_t bytes_read = 0; forwarder != end && bytes_read < bytes_in_chunk;) {
            chunk_data.push_back(*forwarder);
            const std::size_t delim_bytes = 1;
            bytes_read += std::to_string(*forwarder).size() + delim_bytes;
            forwarder = std::next(forwarder);
        }
        return std::move(chunk_data);
    }

    std::vector<ChunksInfo> split_source_in_chunks(const std::filesystem::path &path, std::size_t bytes_in_chunk) {
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

        // Using structure because algorithm may need more fields in the future, so using only std::string isn't enough
        std::vector<ChunksInfo> chunks_meta;
        for (std::size_t chunk_index = 0; forwarder != end; ++chunk_index) {
            auto next_chunk = get_chunk(forwarder, bytes_in_chunk);
            const std::filesystem::path chunk_file_name = temp_directory.string() + std::to_string(chunk_index);
            std::fstream chunk_file(chunk_file_name, std::fstream::out);
            std::ostream_iterator<std::size_t> chunk_file_iterator(chunk_file, " ");
            std::sort(next_chunk.begin(), next_chunk.end());
            std::move(next_chunk.begin(), next_chunk.end(), chunk_file_iterator);
            chunks_meta.emplace_back(chunk_file_name);
        }
        //    std::filesystem::remove_all(temp_directory);
        return std::move(chunks_meta);
    }

    void partially_sort(const std::vector<ChunksInfo> &chunks_meta, std::size_t bytes_restriction) {
        const std::filesystem::path sorted_path = "resources/handled.txt";
        std::fstream sorted_file(sorted_path, std::fstream::out);
        if (!sorted_file.is_open()) {
            throw std::runtime_error("Cannot open file with result buffer");
        }

        std::vector<std::fstream> chunks_info;
        std::for_each(chunks_meta.begin(), chunks_meta.end(), [&chunks_info](const ChunksInfo &meta) {
            std::fstream chunk_file(meta.chunk_name, std::fstream::in);
            chunks_info.emplace_back(std::move(chunk_file));
        });

        const std::size_t chunk_size = bytes_restriction / chunks_info.size();
        std::vector<std::size_t> proxy_buffer;
        std::for_each(chunks_info.begin(), chunks_info.end(), [&proxy_buffer, chunk_size](std::fstream& chunk_file) {
            std::istream_iterator<std::size_t> iter(chunk_file);
            std::vector<std::size_t> chunk_data = get_chunk(iter, chunk_size);
            std::move(chunk_data.begin(), chunk_data.end(), std::back_inserter(proxy_buffer));
        });

        std::sort(proxy_buffer.begin(), proxy_buffer.end());
        std::ostream_iterator<std::size_t> handled_iterator(sorted_file, " ");
        std::move(proxy_buffer.begin(), proxy_buffer.end(), handled_iterator);
    }

    void external_sort(const std::filesystem::path &path, std::size_t bytes_restriction) {
        const auto meta = custom::split_source_in_chunks(path, bytes_restriction);
        partially_sort(meta, bytes_restriction);
    }

}// namespace custom


int main(int argc, char *argv[]) {
    const std::filesystem::path path = "resources/data.txt";
    const std::size_t bytes_restriction = 10;
    custom::external_sort(path, bytes_restriction);
    return 0;
}