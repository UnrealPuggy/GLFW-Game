#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "pug.png.h"
#include "shaders/fragment.glsl.h"
#include "shaders/vertex.glsl.h"
#include <cmath>
const char *vertexShaderSource = (char *)src_shaders_vertex_glsl;
const char *fragmentShaderSource = (char *)src_shaders_fragment_glsl;
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
int main()
{
    // GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current*/
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    /**
     * Vertex Buffer Object, stores the vertex data(positions, texture coords)
     */
    unsigned int VBO;
    /**
     * (Vertex Array Object) stores how vertex attributes are laid out (the “recipe” for drawing)
     */
    unsigned int VAO;
    /**
     * (Element Buffer Object) stores indices (which vertices form each triangle)
     */
    unsigned int EBO;
    // * This generates one VAOZ
    glGenVertexArrays(1, &VAO);
    // * This generates one VBO or EBO
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // * Makes this VAO the current one to record following state.
    glBindVertexArray(VAO);

    // * Binds the VBO — now any vertex-related operations affect it.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // * Uploads vertex data to the GPU.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // * Binds the EBO (for indices).
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //* Uploads index data.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    /**
    * Tells OpenGL how to read vertex data:

    * 0 → attribute location (matches layout(location=0) in vertex shader)

    * 3 → number of components (x,y,z)

    * GL_FLOAT → type

    * stride → 3 floats apart

    * (void)0* → offset
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    //* Enables the attribute so it’s actually used by the shader.
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    // * Enable Wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        //* Sets the background color.
        glClearColor(.1, .1, .1, 1);
        //* Clears the screen to the set color.
        glClear(GL_COLOR_BUFFER_BIT);
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // Tells OpenGL to use your linked vertex+fragment shaders for the next draw.
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glUseProgram(shaderProgram);
        //*Binds your geometry definition.
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        /*
        Draws the indexed geometry:

        GL_TRIANGLES → draw triangles

        6 → number of indices

        GL_UNSIGNED_INT → type of each index

        0 → offset into the EBO
        */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}