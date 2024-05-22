#include "../../engine/src/graphics.hpp"
#include "../include/GLFW/glfw3.h"
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const int screen_width = 800;
const int screen_height = 600;

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialise GLFW" << std::endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Window", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Graphics g((GLADloadproc)glfwGetProcAddress, screen_width, screen_height);

    auto cat = g.loadImage("example/resources/cat.png");
    auto container = g.loadImage("example/resources/container.jpg");

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        g.clearBackground(TEAL);

        g.drawImage(container, 250, 250, 0.5, 0.5, (float)glfwGetTime());

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