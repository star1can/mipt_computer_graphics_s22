#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


#include "Window.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include "GameEngine.hpp"


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

    glClearColor(0.f, 0.f, 1.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    Model zombie("assets/models/zombie.obj",
                 "assets/textures/zombie.DDS",
                 "assets/shaders/TransformVertexShader.vertexshader",
                 "assets/shaders/TextureFragmentShader.fragmentshader");

    Model grass("assets/models/cube.obj",
                "assets/textures/grass_hd.DDS",
                "assets/shaders/TransformVertexShader.vertexshader",
                "assets/shaders/TextureFragmentShader.fragmentshader");
    Model sky("assets/models/sky.obj",
                   "assets/textures/sky16.DDS",
                   "assets/shaders/TransformVertexShader.vertexshader",
                   "assets/shaders/TextureFragmentShader.fragmentshader");

    GameEngine game_engine(window,
                           camera,
                           zombie,
                           grass,
                           sky,
                           45,
                           100);

    /// Draw loop

    // - Loop until the ESC key was pressed or the red cross was not pressed
    while (/*ESC button*/ glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && /*red cross*/ !glfwWindowShouldClose(window.GetWindow()))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffer to suppress flickering

        game_engine.Update();

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