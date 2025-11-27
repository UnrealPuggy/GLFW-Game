#version 330 core
in vec3 aPos; // the position variable has attribute position 0
// out vec3 vertexPos; // specify a color output to the fragment shader
out vec4 vertexColor; // specify a color output to the fragment shader
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
void main()
{
    vec3 pos = aPos;
    // pos.y *= sin(time +aPos.x*5.0+ aPos.z*7.0);
    // float r =  (aPos.x*aPos.x)+(aPos.z*aPos.z);
    pos.y += sin(pos.x+pos.z + time * 2)/4;
    // pos.y -=r/32;
    gl_Position = projection * view * model * vec4(pos, 1.0); // see how we directly give a vec3 to vec4's constructor
    // vertexPos = aPos;

    //  ----- Time Version -------

    // vertexColor = vec4(
    //     sin(aPos.x * 10.0 + time) * 0.5 + 0.5,
    //     sin(aPos.y * 10.0 + time) * 0.5 + 0.5,
    //     sin(aPos.z * 10.0 + time) * 0.5 + 0.5,
    //     1.0
    // );
    // ---- Not Time Version ----
     vertexColor = vec4(
        sin(aPos.x * 10.0 ) * 0.5 + 0.5,
        sin(aPos.y * 10.0 ) * 0.5 + 0.5,
        sin(aPos.z * 10.0 ) * 0.5 + 0.5,
        1.0
    );
    // vertexColor = vec4(aPos * 0.5 + 0.5, 1.0);

    // vertexColor = vec4(aPos, 1); // set the output variable to a dark-red color
}