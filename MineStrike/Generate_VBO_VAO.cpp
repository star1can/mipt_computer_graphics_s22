#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Generate_VBO_VAO.hpp"


GLuint Generatevertex_buffer_object()
{
    GLuint vertex_buffer_object;
    glGenBuffers(1, &vertex_buffer_object);
    return vertex_buffer_object;
}

GLuint Generatevertex_acess_object(GLuint vertex_buffer_object, const GLfloat *vertex_array, int size)
{
    GLuint vertex_acess_object;                 // stores setting we set in for vertex_buffer_object
    glGenVertexArrays(1, &vertex_acess_object); // gen 1 vertex_acess_object object inside OpenGL library (in C we can't create custom objects)

    glBindVertexArray(vertex_acess_object); // use this vertex_acess_object
    // _____________________________________________________________
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);               // use this vertex_buffer_object
    glBufferData(GL_ARRAY_BUFFER, size, vertex_array, GL_STATIC_DRAW); // bind vertex_buffer_object and according array of vertexes
    glVertexAttribPointer(                                             // tell OpenGL how to parse vertex buffer
        0,                                                             // just a random number used in shaders to determine which input corresponds to current buffer
        3,                                                             // array size
        GL_FLOAT,                                                      // array type
        GL_FALSE,                                                      // normalized
        0,                                                             // stride - step in which to read buffer
        (void *)0                                                      // array buffer offset - if data in buffer starts from some specific position
    );
    glEnableVertexAttribArray(0);
    // _____________________________________________________________
    glBindVertexArray(0);

    return vertex_acess_object;
}

GLuint GenerateVAO(
    const std::vector<glm::vec3> &vertices,
    const std::vector<glm::vec2> &uvs,
    const std::vector<glm::vec3> &normals)
{
    GLuint vertex_acess_object;                 // stores setting we set in for vertex_buffer_object
    glGenVertexArrays(1, &vertex_acess_object); // gen 1 vertex_acess_object object inside OpenGL library (in C we can't create custom objects)

    GLuint vertex_buffer_object;
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    GLuint uvs_buffer_object;
    glGenBuffers(1, &uvs_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    GLuint normals_buffer_object;
    glGenBuffers(1, &normals_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, normals_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    glBindVertexArray(vertex_acess_object); // use this vertex_acess_object
    // _____________________________________________________________

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);                                              // use this vertex_buffer_object
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW); // bind vertex_buffer_object and according array of vertexes
    glVertexAttribPointer(                                                                            // tell OpenGL how to parse vertex buffer
        0,                                                                                            // just a random number used in shaders to determine which input corresponds to current buffer
        3,                                                                                            // array size
        GL_FLOAT,                                                                                     // array type
        GL_FALSE,                                                                                     // normalized
        0,                                                                                            // stride - step in which to read buffer
        (void *)0                                                                                     // array buffer offset - if data in buffer starts from some specific position
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_buffer_object);
    glVertexAttribPointer(
        1,        // attribute
        2,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normals_buffer_object);
    glVertexAttribPointer(
        2,        // attribute
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    // _____________________________________________________________
    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    return vertex_acess_object;
}