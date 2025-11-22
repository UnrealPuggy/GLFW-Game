#version 330 core
in vec3 aPos; // the position variable has attribute position 0
// out vec3 vertexPos; // specify a color output to the fragment shader
out vec4 vertexColor; // specify a color output to the fragment shader
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    // vertexPos = aPos;
    vertexColor = vec4(aPos, 1); // set the output variable to a dark-red color
}