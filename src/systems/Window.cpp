#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"

GLFWwindow* Window::window;

int Window::width  = 720;
int Window::height = 480;

int Window::initialize(int width, int height, const char* title) {
    Window::width  = width;
    Window::height = height;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Error with initializing GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Error with initializing GLEW" << std::endl;
        return -1;
    }

    glViewport(0, 0, width, height);

    return 0;
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Window::setShouldClose(bool flag) {
    glfwSetWindowShouldClose(window, flag);
}

void Window::setCursorMode(int mode) {
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
}

void Window::terminate() {
    glfwTerminate();
}