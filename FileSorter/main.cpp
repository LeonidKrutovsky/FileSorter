#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

namespace fs = std::filesystem;

std::string read_file(const fs::path & path)
{
    std::ifstream file(path.u8string());
    std::string buffer(fs::file_size(path), ' ');
    buffer.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
    return buffer;
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
    std::cout << buffer << std::endl;
    return 0;
}
