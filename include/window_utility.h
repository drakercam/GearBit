#ifndef WINDOW_UTILITY_H
#define WINDOW_UTILITY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "math_utility.h"

static inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

struct Window
{
    GLFWwindow* w;
    int width, height;
    std::string title;
    float fov;
    float aspect;
};

static inline Window Window_Init(int w, int h, float fov, std::string t)
{
    Window window;
    window.w = NULL;
    window.width = w;
    window.height = h;
    window.title = t;
    window.fov = Math_DegToRad(fov);
    window.aspect = float(window.width) / float(window.height);

    return window;
}

static inline int Window_Generate(Window& window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.w = glfwCreateWindow(window.width, window.height, window.title.c_str(), NULL, NULL);
    if (window.w == NULL)
    {
        std::cout << "Failed ot create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window.w);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window.w, framebuffer_size_callback);

    return 0;
}

static inline bool Window_ShouldClose(Window window)
{
    return !glfwWindowShouldClose(window.w);
}

static inline void Window_SwapBuffers(Window window)
{
    glfwSwapBuffers(window.w);
}

static inline void Window_PollEvents()
{
    glfwPollEvents();
}

static inline void Window_Delete()
{
    glfwTerminate();
}

static inline void Window_EnableDepthTesting()
{
    glEnable(GL_DEPTH_TEST);
}

static inline void Window_Clear(Vector4 color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#endif