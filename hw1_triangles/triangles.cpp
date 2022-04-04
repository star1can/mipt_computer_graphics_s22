// Include standard headers
#include <stdio.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

#include <common/shader.hpp>


using namespace glm;



void DrawTriangles(GLuint vertex_buffer, GLuint shader, GLint start_vertex_index) {
    glUseProgram(shader);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);
    glDrawArrays(GL_TRIANGLES, start_vertex_index, 3);
    glDisableVertexAttribArray(0);
}


int main() {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // smooth
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow *window = glfwCreateWindow(1024, 768, "HW-1 Triangles", NULL, NULL);

    if (window == NULL) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // background
    glClearColor(0.33f, 0.86f, 0.44f, 0.5f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders

    GLuint shaders[2];
    shaders[0] = LoadShaders("VertexShader.vertexshader",
                                      "FirstTriangleFragmentShader.fragmentshader");

    shaders[1] = LoadShaders("VertexShader.vertexshader",
                                      "SecondTriangleFragmentShader.fragmentshader");


    static const GLfloat vertex_buffer_data[] = {
            -0.95f, -0.8f, 0.4f,
            0.0f, 0.9f, -0.5f,
            0.95f, -0.8f, 0.4f,
            /////////////////
            -0.95f, 0.8f, 0.0f,
            0.0f, -0.9f, 0.0f,
            0.95f, 0.8f, 0.0f,
    };

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    do {

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangles
        DrawTriangles(vertex_buffer, shaders[0], 0);
        DrawTriangles(vertex_buffer, shaders[1], 3);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(shaders[0]);
    glDeleteProgram(shaders[1]);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

