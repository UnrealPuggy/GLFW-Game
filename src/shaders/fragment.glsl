#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
uniform vec4 ourColor;
void main()
{
    FragColor = clamp(vertexColor + ourColor, 0.0, 1.0);
// FragColor = vertexColor+ourColor/10;
} 