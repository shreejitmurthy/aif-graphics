#include "../../engine/src/graphics.hpp"
#include "../include/GLFW/glfw3.h"
#include <cmath>

/* TODO:
    -  Text rendering
*/

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

    auto cat = g.loadImage("example/resources/majestic_cat.jpeg");

    float x = screen_width/2;
    float y = screen_height/2;

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            x += 1;
        }  else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            x -= 1;
        }
        if (glfwGetKey(window, GLFW_KEY_UP)) {
            y -= 1;
        } else if (glfwGetKey(window, GLFW_KEY_DOWN)) {
            y += 1;
        }

        g.clearBackground(TEAL);

        cat.rotate((float)glfwGetTime());
        cat.scale(sin((double)glfwGetTime()), sin((double)glfwGetTime()) / 2.f);
        g.drawImage(cat, x, y);

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