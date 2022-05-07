#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
private:
  GLFWwindow *window;
  int width;
  int height;

public:
  Window(const char *title, int width, int height);

  GLFWwindow *GetWindow();

  GLuint GetWidth();

  GLuint GetHeight();

  // - Set drawing context to current window
  void MakeContextCurrent();

  // This function binds VAO only once - before drawing all the objects
  // TODO: а стоит ли делать функцию рисующую сразу все объекты с одним VAO ????
  // void Draw(const Model &model, const Camera &camera);
};