// Include standard headers
#include <stdio.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>

using namespace glm;


void DrawTriangles(GLuint vertex_buffer, GLuint shader, GLint start_vertex_index, GLint matrix_id, GLfloat *mvp) {
    glUseProgram(shader);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, mvp);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);
    glDrawArrays(GL_TRIANGLES, start_vertex_index, 3);
    glDisableVertexAttribArray(0);
}


glm::mat4 CalculateMVP(double time, int64_t speed) {
    double angle = (double) ((size_t) (time * 1000) % speed) / speed * 2 * glm::pi<double>();

    glm::vec3 camera_pos = glm::vec3(glm::cos(angle) * 2, 1.5, glm::sin(angle) * 2);
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.1f, 20.0f);
    glm::mat4 view = glm::lookAt(camera_pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 model = glm::mat4(1.0f);

    return projection * view * model;
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
    GLFWwindow *window = glfwCreateWindow(1024, 768, "MOVE!", NULL, NULL);

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


    GLint matrix_ids[2];

    matrix_ids[0] = glGetUniformLocation(shaders[0], "MVP");
    matrix_ids[1] = glGetUniformLocation(shaders[1], "MVP");

    static const GLfloat vertex_buffer_data[] = {
            -0.8, -0.4, 0.9,
            0, 0.8, -0.9,
            0.8, -0.4, 0.9,
            /////////////
            -0.8, -0.4, -0.9,
            0, 0.8, 0.9,
            0.8, 0.4, -0.9,
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

        // calculate mvp
        glm::mat4 mvp = CalculateMVP(glfwGetTime(), 10000);

        // Draw triangles
        DrawTriangles(vertex_buffer, shaders[0], 0, matrix_ids[0], &mvp[0][0]);
        DrawTriangles(vertex_buffer, shaders[1], 3, matrix_ids[1], &mvp[0][0]);

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

