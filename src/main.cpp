// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
#include <cstdio>
#include "shaders/fragment.glsl.h"
#include "shaders/vertex.glsl.h"
#include <cmath>
#include "libs/common.hpp"
// #include "libs/glm/glm/glm.hpp"
// #include "glm/gtc/matrix_transform.hpp"
// #include <glm/gtc/type_ptr.hpp>
// #include "stb_image.h"

#include "assets/pug_icon_high_res.jpg.h"

const char *vertexShaderSource = (char *)src_shaders_vertex_glsl;
const char *fragmentShaderSource = (char *)src_shaders_fragment_glsl;
bool mouseCaptured = true;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f; // start facing -Z
float pitch = 0.0f;
float lastX = 320, lastY = 240;
float fov = 45.0f;
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (!mouseCaptured)
        return;
    static bool firstMouse = true;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed: y goes from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

int main()
{
    if (!glfwInit())
        return -1;

    // Enable Anti Aliasing
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_MAXIMIZED, true); // Request maximized window

    GLFWwindow *window = glfwCreateWindow(640, 480, "CUBE", NULL, NULL);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
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
        // Front face
        -0.5f, -0.5f, 0.5f, // 0: bottom-left
        0.5f, -0.5f, 0.5f,  // 1: bottom-right
        0.5f, 0.5f, 0.5f,   // 2: top-right
        -0.5f, 0.5f, 0.5f,  // 3: top-left
        // Back face
        -0.5f, -0.5f, -0.5f, // 4: bottom-left
        0.5f, -0.5f, -0.5f,  // 5: bottom-right
        0.5f, 0.5f, -0.5f,   // 6: top-right
        -0.5f, 0.5f, -0.5f   // 7: top-left
    };

    // ----------------- Indices -----------------
    unsigned int indices[] = {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Right face
        1, 5, 6,
        6, 2, 1,
        // Back face
        5, 4, 7,
        7, 6, 5,
        // Left face
        4, 0, 3,
        3, 7, 4,
        // Top face
        3, 2, 6,
        6, 7, 3,
        // Bottom face
        4, 5, 1,
        1, 0, 4};

    unsigned int VAO, VBO, EBO;
    // This gens the unique ID
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    int posLoc = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(posLoc);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnable(GL_DEPTH_TEST); // Enable depth testing
    static float lastTime = -1 / 60.0f;

    // ----------------- Render loop -----------------
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !mouseCaptured)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastX = (float)xpos;
            lastY = (float)ypos;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hide and capture mouse
            mouseCaptured = true;
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && mouseCaptured)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // release mouse
            mouseCaptured = false;
        }

        // Clear screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Rainbow color
        float timeValue = glfwGetTime();
        float dt = timeValue - lastTime;
        lastTime = timeValue;
        // printf("%f\n", 1 / dt);
        float r = (sin(timeValue * 2.0f) + 1.0f) / 2.0f;
        float g = (sin(timeValue * 2.0f + 2.0f) + 1.0f) / 2.0f;
        float b = (sin(timeValue * 2.0f + 4.0f) + 1.0f) / 2.0f;

        int vertexColorLocation = glGetUniformLocation(shaderProgram, "time");
        glUniform1f(vertexColorLocation, glfwGetTime());
        // glUniform4f(vertexColorLocation, r, g, b, 1.0f);

        float cameraSpeed = 2.5f * 1.0f / 60 * (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ? 2 : 1); // deltaTime = frame time

        if (mouseCaptured)
        {

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                cameraPos += cameraSpeed * cameraFront;
            if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                cameraPos += cameraSpeed * cameraUp;
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                cameraPos -= cameraSpeed * cameraUp;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                cameraPos -= cameraSpeed * cameraFront;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }

        // ----------------- Uniform matrices -----------------
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 100.0f);
        float radius = 3.0f;            // distance from the rectangle
        float camAngle = glfwGetTime(); // rotate over time
        float camX = sin(camAngle) * radius;
        float camZ = cos(camAngle) * radius;
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

        // model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0, 1, 0));

        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        int projLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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
