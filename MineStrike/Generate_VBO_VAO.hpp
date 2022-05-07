#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

GLuint GenerateVBO();

GLuint GenerateVAO(GLuint VBO, const GLfloat *vertex_array, int size);

GLuint GenerateVAO(const std::vector<glm::vec3> &vertices,
                   const std::vector<glm::vec2> &uvs,
                   const std::vector<glm::vec3> &normals);