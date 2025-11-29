#include "Shader.hpp"
#include <fstream>
#include <vector>

Shader::Shader(const char *vs, const char *fs)
{
}
char *read_bytes(const char *path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Failed to open file");

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0);

    char data[size];
    file.read(data, size);

    return data;
}