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

glm::mat4 CalculateMVP(double time, int64_t speed) {
    double angle = (double) ((size_t) (time * 1000) % speed) / speed * 2 * glm::pi<double>();

    glm::vec3 camera_pos = glm::vec3(glm::cos(angle) * 60, glm::cos(angle) * 40, glm::sin(angle) * 60);
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.1f, 200.0f);
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
    GLFWwindow *window = glfwCreateWindow(1024, 768, "Hello from USSR", NULL, NULL);

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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders

    GLuint shader = LoadShaders("TransformVertexShader.vertexshader",
                                "ColorFragmentShader.fragmentshader");;


    GLint matrix_id = glGetUniformLocation(shader, "MVP");

    float z = 12;
    static const GLfloat vertex_buffer_data[] = {
            ////base////
            // top
            -9, 13, 0,
            0, 40, 0,
            9, 13, 0,
            // right
            9, 13, 0,
            40, 13, 0,
            15, -5, 0,
            // right bottom
            15, -5, 0,
            25, -34, 0,
            0, -16, 0,
            // left bottom
            0, -16, 0,
            -25, -34, 0,
            -15, -5, 0,
            // left
            -15, -5, 0,
            -40, 13, 0,
            -9, 13, 0,
            ////////////

            ////center////
            // top
            -9, 13, 0,
            9, 13, 0,
            0, 0, 0,
            // right
            9, 13, 0,
            15, -5, 0,
            0, 0, 0,
            // right bottom
            15, -5, 0,
            0, -16, 0,
            0, 0, 0,
            // left bottom
            0, -16, 0,
            -15, -5, 0,
            0, 0, 0,
            // left
            -15, -5, 0,
            -9, 13, 0,
            0, 0, 0,
            ////////////

            ////front////
            // top_left
            -9, 13, 0,
            0, 40, 0,
            0, 0, z,
            // top_right
            9, 13, 0,
            0, 40, 0,
            0, 0, z,
            // right_left
            9, 13, 0,
            40, 13, 0,
            0, 0, z,
            // right_right
            15, -5, 0,
            40, 13, 0,
            0, 0, z,
            // right_bottom_left
            15, -5, 0,
            25, -34, 0,
            0, 0, z,
            // right_bottom_right
            0, -16, 0,
            25, -34, 0,
            0, 0, z,
            // left_bottom_left
            0, -16, 0,
            -25, -34, 0,
            0, 0, z,
            // left_bottom_right
            -15, -5, 0,
            -25, -34, 0,
            0, 0, z,
            // left_left
            -15, -5, 0,
            -40, 13, 0,
            0, 0, z,
            // left_right
            -9, 13, 0,
            -40, 13, 0,
            0, 0, z,
            ////////////

            ////bottom////
            // top_left
            -9, 13, 0,
            0, 40, 0,
            0, 0, -z,
            // top_right
            9, 13, 0,
            0, 40, 0,
            0, 0, -z,
            // right_left
            9, 13, 0,
            40, 13, 0,
            0, 0, -z,
            // right_right
            15, -5, 0,
            40, 13, 0,
            0, 0, -z,
            // right_bottom_left
            15, -5, 0,
            25, -34, 0,
            0, 0, -z,
            // right_bottom_right
            0, -16, 0,
            25, -34, 0,
            0, 0, -z,
            // left_bottom_left
            0, -16, 0,
            -25, -34, 0,
            0, 0, -z,
            // left_bottom_right
            -15, -5, 0,
            -25, -34, 0,
            0, 0, -z,
            // left_left
            -15, -5, 0,
            -40, 13, 0,
            0, 0, -z,
            // left_right
            -9, 13, 0,
            -40, 13, 0,
            0, 0, -z,
            ////////////
    };

    GLfloat color_buffer_data[90 * 3];

    for (size_t i = 0; i < 90 * 3; i += 3) {
        color_buffer_data[i] = 1.f;
        color_buffer_data[i + 1] = 0.f;
        color_buffer_data[i + 2] = 0.f;
    }

    for (size_t i = 10 * 3 * 3; i < 90 * 3; i += 3 * 3) {
        color_buffer_data[i] = 0.2f;
        color_buffer_data[i + 1] = 0.f;
        color_buffer_data[i + 2] = 0.f;

        color_buffer_data[i + 6] = 0.2f;
        color_buffer_data[i + 7] = 0.f;
        color_buffer_data[i + 8] = 0.f;
    }

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    GLuint color_buffer;
    glGenBuffers(1, &color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data),
                 color_buffer_data, GL_STATIC_DRAW);

    do {

        glm::mat4 mvp = CalculateMVP(glfwGetTime(), 20000);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(shader);

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

        // Draw triangles
        glDrawArrays(GL_TRIANGLES, 0, 90);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &color_buffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(shader);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

