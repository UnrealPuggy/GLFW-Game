#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "pug.rgb.h"
int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current*/
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Image?
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 8, 0, GL_RGB, GL_UNSIGNED_BYTE, src_pug_rgb);
    /* Loop until the user closes the window asd*/

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(.1, .1, .1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_TEXTURE_2D);
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex2f(-0.5f, -0.5f);
        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glVertex2f(0.5f, -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        glVertex2f(0.0f, 0.5f);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}