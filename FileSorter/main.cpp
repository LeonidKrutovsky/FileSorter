#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <thread>
#include <future>
#include <chrono>

#if defined(__GNUG__)
 #include <parallel/algorithm>
#endif

namespace fs = std::filesystem;

std::string read_file(const fs::path & path)
{
    std::ifstream file(path.u8string());
    std::string buffer(fs::file_size(path), ' ');
    buffer.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
    file.close();
    return buffer;
}


void write_file(const fs::path & path, const std::vector<std::string_view> & lines)
{
    std::ofstream file(path.u8string());
    std::copy( lines.cbegin(), lines.cend(),
               std::ostream_iterator<std::string_view>(file, "\n") );
    file.close();
}


std::vector<std::string_view> find_lines(std::string_view str)
{
    std::vector<std::string_view> lines;
    std::string_view::size_type start(0), pos(0);
    do
    {
        pos = str.find('\n', start);
        lines.emplace_back(str.substr(start, pos - start));
        start = pos+1;
    }
    while (pos != std::string::npos && start != str.size());
    return lines;
}

std::vector<std::string_view> find_and_sort_lines(std::string_view buffer)
{
    auto lines = find_lines(buffer);
    std::sort(lines.begin(), lines.end());
    return lines;
}


std::vector<std::string_view> async_sorted_lines(std::string_view buffer, const std::string_view::size_type chunk_size)
{
    auto pos = buffer.find('\n', chunk_size);

    if (pos == std::string::npos)
    {
        return find_and_sort_lines(buffer);
    }

    auto handle = std::async(std::launch::async,
                             async_sorted_lines, buffer.substr(pos+1), chunk_size);

    auto first_chunk = find_and_sort_lines(buffer.substr(0, pos));
    auto last_chunk = handle.get();
    std::vector<std::string_view> result(first_chunk.size() + last_chunk.size());
    std::merge(first_chunk.begin(), first_chunk.end(), last_chunk.begin(), last_chunk.end(), result.begin());
    return result;
}

std::vector<std::string_view> sorted_lines(std::string_view buffer)
{
#if defined(__GNUG__)
    auto lines = find_lines(buffer);
    __gnu_parallel::sort(lines.begin(), lines.end());
    return lines;
#else
    static constexpr std::string_view::size_type treshold = 1024 * 1024 * 4;
    const int max_threads = std::thread::hardware_concurrency();
    const std::string_view::size_type chunk_size = buffer.size() / max_threads;
    if (chunk_size < treshold)
    {
        auto lines = find_lines(buffer);
        std::sort(lines.begin(), lines.end());
        return lines;
    }
    return async_sorted_lines(buffer, chunk_size);
#endif
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " input_file" << " output_file" << std::endl;
        return 1;
    }
    const fs::path input_file_name = argv[1];
    if (!fs::exists(input_file_name))
    {
        std::cerr << "No input file " << input_file_name << std::endl;
    }
    const fs::path output_file_name = argv[2];
    const auto buffer = read_file(input_file_name);
    const auto lines = sorted_lines(std::string_view(buffer));
    write_file(output_file_name, lines);
    return 0;
}
