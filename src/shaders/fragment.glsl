#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
in vec2 TexCoords;

uniform sampler2D texture1;

uniform vec4 ourColor;
void main()
{
        FragColor = texture(texture1, TexCoords);

    // FragColor = vertexColor;//clamp(vertexColor, 0.0, 1.0);
// FragColor = vertexColor+ourColor/10;
} 