#pragma once
struct Shader
{
    unsigned int shaderProgram;
    unsigned int fragmentShader;
    unsigned int vertexShader;

    Shader(const char *vs, const char *fs);
    void use();
    int getUniformLocation(const char *name);
    int getAttribLocation(const char *name);
};