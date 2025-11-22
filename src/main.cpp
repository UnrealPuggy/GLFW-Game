#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "shaders/fragment.glsl.h"
#include "shaders/vertex.glsl.h"
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "assets/pug_icon_high_res.jpg.h"

const char *vertexShaderSource = (char *)src_shaders_vertex_glsl;
const char *fragmentShaderSource = (char *)src_shaders_fragment_glsl;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    if (!glfwInit())
        return -1;

    GLFWwindow *window = glfwCreateWindow(640, 480, "CUBE", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Set window icon
    GLFWimage images[1];
    images[0].pixels = stbi_load_from_memory(
        src_assets_pug_icon_high_res_jpg,
        src_assets_pug_icon_high_res_jpg_len,
        &images[0].width, &images[0].height, 0, 4);
    if (!images[0].pixels)
        printf("Image not loaded\n");
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // ----------------- Compile shaders -----------------
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram); // Activate shader program before setting uniforms

    // ----------------- Vertex data -----------------
    float vertices[] = {
        0.5f, 0.5f, 0.0f,    // top right
        0.5f, -0.5f, 0.0f,   // bottom right
        -0.5f, -0.5f, -0.5f, // bottom left
        -0.5f, 0.5f, 0.0f    // top left
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3};

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    int posLoc = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(posLoc);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0); // usually left bound until rendering

    // ----------------- Uniform matrices -----------------
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(2.0f), glm::vec3(0, 1, 0));

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glEnable(GL_DEPTH_TEST); // Enable depth testing

    // ----------------- Render loop -----------------
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Rainbow color
        float timeValue = glfwGetTime();
        float r = (sin(timeValue * 2.0f) + 1.0f) / 2.0f;
        float g = (sin(timeValue * 2.0f + 2.0f) + 1.0f) / 2.0f;
        float b = (sin(timeValue * 2.0f + 4.0f) + 1.0f) / 2.0f;

        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, r, g, b, 1.0f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
