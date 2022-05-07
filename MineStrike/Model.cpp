#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/objloader.hpp>

#include "Model.hpp"
#include "Generate_VBO_VAO.hpp"

Model::Model(const char *obj_file,
             const char *dds_file,
             const char *vertex_shader_file,
             const char *fragment_shader_file,
             const char *glsl_mvp_matrix_name,
             const char *glsl_texture_name)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    loadOBJ(obj_file, vertices, uvs, normals);

    VAO = GenerateVAO(vertices, uvs, normals);
    shader_program = LoadShaders(vertex_shader_file, fragment_shader_file);
    texture = loadDDS(dds_file);
    glsl_mvp_matrix = glGetUniformLocation(shader_program, glsl_mvp_matrix_name);
    glsl_texture = glGetUniformLocation(shader_program, glsl_texture_name);
    triangles_count = vertices.size();
}