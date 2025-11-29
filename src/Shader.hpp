#pragma once
class Shader
{
    unsigned int shaderProgram;
    unsigned int fragmentShader;
    unsigned int vertexShader;

public:
    Shader(const char *vs, const char *fs);
};