#include <iostream>
#include "../../Engine/graphics.hpp"
#include "../../Engine/vendor/shader_s.h"
#include "../include/GLFW/glfw3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialise GLFW" << std::endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Window", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Graphics g((GLADloadproc)glfwGetProcAddress);
    
    Shader ourShader("example/resources/shaders/texture.vs", "example/resources/shaders/texture.fs");

    auto texture = g.loadImage("example/resources/container.jpg");

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        g.clearBackground(TEAL);

        ourShader.use();
        g.drawImage(texture);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    setVPT(width, height);
}