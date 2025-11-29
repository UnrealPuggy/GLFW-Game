#include "Shader.hpp"
#include <fstream>
#include <vector>
#include "libs/common.hpp"
#include <cstdio>
std::vector<char> read_bytes(const char *path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
        throw std::runtime_error("Failed to open file");

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0);

    std::vector<char> data(size);
    file.read(data.data(), size);
    return data; // safe, vector owns memory
}
Shader::Shader(const char *vs, const char *fs)
{
    std::vector<char> vertexSourceVec = read_bytes(vs);
    vertexSourceVec.push_back('\0');
    const char *vertexShaderSource = vertexSourceVec.data();

    std::vector<char> fragmentSourceVec = read_bytes(fs);
    fragmentSourceVec.push_back('\0');
    const char *fragmentShaderSource = fragmentSourceVec.data();
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {

        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Vertex shader compilation error: %s\n", infoLog);
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    success = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {

        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Fragment Shader compilation error: %s\n", infoLog);
    }
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use()
{
    glUseProgram(shaderProgram);
}

int Shader::getUniformLocation(const char *name)
{
    return glGetUniformLocation(shaderProgram, name);
}

int Shader::getAttribLocation(const char *name)
{
    return glGetAttribLocation(shaderProgram, name);
}
