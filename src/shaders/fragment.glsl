#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
uniform vec4 ourColor;
void main()
{
    FragColor = vertexColor;//clamp(vertexColor, 0.0, 1.0);
// FragColor = vertexColor+ourColor/10;
} 