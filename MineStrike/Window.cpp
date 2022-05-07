#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"

Window::Window(const char *title, int width_, int height_)
{
  width = width_;
  height = height_;

  // - Setup window
  glfwWindowHint(GLFW_SAMPLES, 4);               // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

  // - Create window
  window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!window)
  {
    std::cout << "glfwCreateWindow() Error";
    glfwTerminate();
    return;
  }

  // - Set window closing on ESC button
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

GLFWwindow *Window::GetWindow()
{
  return window;
}

GLuint Window::GetWidth()
{
  return width;
}

GLuint Window::GetHeight()
{
  return height;
}

// - Set drawing context to current window
void Window::MakeContextCurrent()
{
  glfwMakeContextCurrent(window);
}