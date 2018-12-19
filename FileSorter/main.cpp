#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <assert.h>
#include <algorithm>

namespace fs = std::filesystem;

std::string read_file(const fs::path & path)
{
    std::ifstream file(path.u8string());
    std::string buffer(fs::file_size(path), ' ');
    buffer.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
    return buffer;
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

void sort_lines(std::vector<std::string_view> & lines)
{
    std::sort(lines.begin(), lines.end());  //TODO: parallel sort
}

void test_find_lines();
void test_sort_lines();

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
    std::cout << buffer << std::endl;

    test_find_lines();

    return 0;
}

void test_find_lines()
{
    const std::string_view test("\n12345\n12345\n");
    std::vector<std::string_view> expected;
    expected.push_back(test.substr(0, 0));
    expected.push_back(test.substr(1, 5));
    expected.push_back(test.substr(7, 5));

    auto result = find_lines(test);
    assert(result == expected);
}

void test_sort_lines()
{
    std::vector<std::string_view> test;
    test.emplace_back("1111");
    test.emplace_back("5555");
    test.emplace_back("AAAA");

    auto expected = test;
    std::sort(expected.begin(), expected.end());
    sort_lines(test);
    assert(test == expected);
}
