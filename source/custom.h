#pragma once
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

namespace custom {

    template<typename It>
    struct Range {
        It start;
        It end;
    };

    template<typename It, typename OutputIterator>
    void merge_n(std::vector<Range<It>> &containers, OutputIterator out) {
        auto iteration_over = [](const Range<It> &range) {
            return range.start == range.end;
        };

        while (!std::all_of(containers.begin(), containers.end(), iteration_over)) {
            auto min_range = std::min_element(containers.begin(), containers.end(), [](Range<It> &first, Range<It> &second) {
                return *first.start < *second.start;
            });
            *out = *min_range->start;

            min_range->start = std::next(min_range->start);
            const bool range_done = min_range->start == min_range->end;
            if (range_done) {
                containers.erase(min_range);
            }
            out++;
        }
    }

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

    std::vector<std::string> split_source_in_chunks(const std::filesystem::path &path, const std::filesystem::path &temp_directory, std::size_t bytes_in_chunk) {
        if (bytes_in_chunk <= 0) {
            throw std::runtime_error("Bad chunk size");
        }

        std::fstream file(path, std::fstream::in);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open source file");
        }

        std::istream_iterator<std::size_t> forwarder(file);
        std::istream_iterator<std::size_t> end;

        std::vector<std::string> chunks_names;
        for (std::size_t chunk_index = 0; forwarder != end; ++chunk_index) {
            auto next_chunk = get_chunk(forwarder, bytes_in_chunk);
            const std::filesystem::path chunk_file_name = temp_directory.string() + std::to_string(chunk_index);
            std::fstream chunk_file(chunk_file_name, std::fstream::out);
            std::ostream_iterator<std::size_t> chunk_file_iterator(chunk_file, " ");
            std::sort(next_chunk.begin(), next_chunk.end());
            std::move(next_chunk.begin(), next_chunk.end(), chunk_file_iterator);
            chunks_names.emplace_back(chunk_file_name);
        }
        return std::move(chunks_names);
    }

    template<typename Container>
    void partially_sort(Container &&chunks_filenames) {
        const std::filesystem::path sorted_path = "resources/handled.txt";
        std::fstream sorted_file(sorted_path, std::fstream::out);
        if (!sorted_file.is_open()) {
            throw std::runtime_error("Cannot open file with result buffer");
        }

        std::vector<std::fstream> chunks_info;
        std::for_each(chunks_filenames.begin(), chunks_filenames.end(), [&chunks_info](const std::string &chunk_filename) {
            std::fstream chunk_file(chunk_filename, std::fstream::in);
            chunks_info.emplace_back(std::move(chunk_file));
        });

        std::vector<Range<std::istream_iterator<std::size_t>>> chunks_iterators;
        std::for_each(chunks_info.begin(), chunks_info.end(), [&chunks_iterators](std::fstream &stream) {
            std::istream_iterator<std::size_t> forwarder(stream);
            std::istream_iterator<std::size_t> end;
            chunks_iterators.push_back({forwarder, end});
        });

        std::ostream_iterator<std::size_t> handled_iterator(sorted_file, " ");
        merge_n(chunks_iterators, handled_iterator);
    }

    void external_sort(const std::filesystem::path &path, std::size_t bytes_restriction) {
        const std::filesystem::path temp_directory = "tmp/";
        std::filesystem::create_directory(temp_directory);

        auto meta = custom::split_source_in_chunks(path, temp_directory, bytes_restriction);
        partially_sort(std::move(meta));

        std::filesystem::remove_all(temp_directory);
    }

}// namespace custom
