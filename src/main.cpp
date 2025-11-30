// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
#include <cstdio>
#include "shaders/fragment.glsl.h"
#include "shaders/vertex.glsl.h"
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION

#include "libs/common.hpp"
#include <fstream>
#include <filesystem>
#include "pug.png.h"
#include "Shader.hpp"

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
void doFile(const char *realPath, const char *src, int size)
{
    std::filesystem::path filePath(realPath);

    // Create directories leading to the file
    std::filesystem::create_directories(filePath.parent_path());

    std::ofstream out(filePath, std::ios::binary);
    if (!out)
    {
        printf("Failed to open file for writing!\n");
        return;
    }

    out.write(src, size);
    out.close();
}

int main()
{

    // Put the extracted files Here
    doFile("cube123/src/shaders/fragment.glsl", fragmentShaderSource, src_shaders_fragment_glsl_len);
    doFile("cube123/src/shaders/vertex.glsl", vertexShaderSource, src_shaders_vertex_glsl_len);
    doFile("cube123/src/pug.png", (char *)src_pug_png, src_pug_png_len);
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

    Shader myShader("cube123/src/shaders/vertex.glsl", "cube123/src/shaders/fragment.glsl");

    // ----------------- Vertex data -----------------
    // Vertices with uV
    float vertices[] = {
        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        // Back face
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        // Left face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        // Right face
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        // Top face
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f};

    // Old Vertices
    // float vertices[] = {
    //     // Front face
    //     -0.5f, -0.5f, 0.5f, // 0: bottom-left
    //     0.5f, -0.5f, 0.5f,  // 1: bottom-right
    //     0.5f, 0.5f, 0.5f,   // 2: top-right
    //     -0.5f, 0.5f, 0.5f,  // 3: top-left
    //     // Back face
    //     -0.5f, -0.5f, -0.5f, // 4: bottom-left
    //     0.5f, -0.5f, -0.5f,  // 5: bottom-right
    //     0.5f, 0.5f, -0.5f,   // 6: top-right
    //     -0.5f, 0.5f, -0.5f   // 7: top-left
    // };

    // ----------------- Indices -----------------

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,       // Front
        4, 5, 6, 6, 7, 4,       // Back
        8, 9, 10, 10, 11, 8,    // Left
        12, 13, 14, 14, 15, 12, // Right
        16, 17, 18, 18, 19, 16, // Top
        20, 21, 22, 22, 23, 20  // Bottom
    };

    // Texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("cube123/src/pug.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                     nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);

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
    int posLoc = myShader.getAttribLocation("aPos"); // glGetAttribLocation(test.shaderProgram, "aPos");
    int uvLoc = myShader.getAttribLocation("aUV");
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(posLoc);

    glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(uvLoc);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnable(GL_DEPTH_TEST); // Enable depth testing
    static float lastTime = -1 / 60.0f;
    glm::mat4 model = glm::mat4(1.0f);
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

        myShader.use();
        glActiveTexture(GL_TEXTURE0); // activate texture unit 0
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(myShader.getUniformLocation("texture1"), 0);
        // glUseProgram(shaderProgram);

        // Rainbow color
        float timeValue = glfwGetTime();
        float dt = timeValue - lastTime;
        lastTime = timeValue;
        // printf("%f\n", 1 / dt);
        float r = (sin(timeValue * 2.0f) + 1.0f) / 2.0f;
        float g = (sin(timeValue * 2.0f + 2.0f) + 1.0f) / 2.0f;
        float b = (sin(timeValue * 2.0f + 4.0f) + 1.0f) / 2.0f;

        int vertexColorLocation = myShader.getUniformLocation("time"); // glGetUniformLocation(test.shaderProgram, "time");
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

        // model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

        model = glm::rotate(model, glm::radians(90.0f * dt), glm::vec3(1, 1, 1));

        int modelLoc = myShader.getUniformLocation("model");     // glGetUniformLocation(test.shaderProgram, "model");
        int viewLoc = myShader.getUniformLocation("view");       // glGetUniformLocation(test.shaderProgram, "view");
        int projLoc = myShader.getUniformLocation("projection"); // glGetUniformLocation(test.shaderProgram, "projection");

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
    glDeleteProgram(myShader.shaderProgram);
    std::filesystem::remove_all("cube123");
    glfwTerminate();
    return 0;
}
