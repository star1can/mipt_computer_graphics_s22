#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #include "Generate_VBO_VAO.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include "GameEngine.hpp"

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/objloader.hpp>
#include <common/text2D.hpp>

glm::mat4 GetModelMatrix()
{
  // Model transformations
  glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
  glm::mat4 rotate = glm::rotate(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(1.0f));

  return glm::mat4(1.0f);
}

int main()
{
  /// Init GLFW
  if (!glfwInit())
  {
    std::cout << "glfwInit() Error";
    return 1;
  }

  /// Create window

  Window window("main window", 1024, 768);
  window.MakeContextCurrent();

  Camera camera(window);

  // Вот тут (или ещё где-то, может в отдельном файле) создай модель куба и шара

  /// Init GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK)
  {
    std::cout << "glewInit() Error";
    glfwTerminate();
    return 1;
  }

  /// Creating objects

  Model zombie("assets/models/zombie.obj",
               "assets/textures/zombie.DDS",
               "assets/shaders/TransformVertexShader.vertexshader",
               "assets/shaders/TextureFragmentShader.fragmentshader");
  Model grass("assets/models/cube.obj",
              "assets/textures/grass.DDS",
              "assets/shaders/TransformVertexShader.vertexshader",
              "assets/shaders/TextureFragmentShader.fragmentshader");
  Model fireball("assets/models/fireball.obj",
                 "assets/textures/fireball.DDS",
                 "assets/shaders/TransformVertexShader.vertexshader",
                 "assets/shaders/TextureFragmentShader.fragmentshader");

  GameEngine game_engine(window,
                         camera,
                         zombie,
                         fireball,
                         grass,
                         40,
                         200);

  initText2D("assets/textures/holstein.DDS");

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  // Cull triangles which normal is not towards the camera
  glEnable(GL_CULL_FACE);

  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  /// Draw loop

  // - Loop until the ESC key was pressed or the red cross was not pressed
  while (/*ESC button*/ glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && /*red cross*/ !glfwWindowShouldClose(window.GetWindow()))
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffer to suppress flickering

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // glBindVertexArray(zombie.VAO);

    // glUseProgram(zombie.shader_program);

    // // Bind our texture in Texture Unit 0
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, zombie.texture); //можно ли перенести это в
    // glUniform1i(zombie.glsl_texture, 0);          // Set our "myTextureSampler" sampler to use Texture Unit 0

    // for (size_t i = 0; i < 360; i += 45)
    // {

    //   glm::mat4 mvp_matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix() * glm::rotate(glm::radians(float(i)), glm::vec3(0.0f, 0.0f, 1.0f));
    //   glUniformMatrix4fv(zombie.glsl_mvp_matrix, 1, GL_FALSE, &mvp_matrix[0][0]); // transfer mvp matrix to shader

    //   glDrawArrays(GL_TRIANGLES, 0, zombie.triangles_count);
    // }

    // glBindVertexArray(0);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    game_engine.Update();
    char text[256];
    sprintf(text, "%.2f sec", glfwGetTime());
    std::string count(std::to_string(42));
    printText2D(count.data(), 10, 500, 100);

    // - Check and call events and swap the buffers
    glfwSwapBuffers(window.GetWindow()); // swaps 2 buffers to avoid showing incomplete buffer
    glfwPollEvents();                    // checks for key pressing or mouse control
  }

  // Delete everything
  // glDeleteBuffers(1, &cube.VBO);
  // glDeleteVertexArrays(1, &cube.VAO);
  // glDeleteProgram(cube.shader_program);
  glfwTerminate();

  return 0;
}