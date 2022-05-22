#include <vector>

#include <GL/glew.h>
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
             const char *glsl_texture_name) {

    loadOBJ(obj_file, vertices_, uvs_, normals_);

    VAO = GenerateVAO(vertices_, uvs_, normals_);
    shader_program = LoadShaders(vertex_shader_file, fragment_shader_file);
    texture = loadDDS(dds_file);
    glsl_mvp_matrix = glGetUniformLocation(shader_program, glsl_mvp_matrix_name);
    glsl_texture = glGetUniformLocation(shader_program, glsl_texture_name);
    triangles_count = vertices_.size();
}

std::vector<glm::vec3>& Model::GetVertices() {
    return vertices_;
}

std::vector<glm::vec3>& Model::GetNormals() {
    return normals_;
}

std::vector<glm::vec2> & Model::GetUvs() {
    return uvs_;
}

void Model::UpdateVAO(
        const std::vector<glm::vec3> &new_vertices,
        const std::vector<glm::vec2> &new_uvs,
        const std::vector<glm::vec3> &new_normals) {
    VAO = GenerateVAO(new_vertices, new_uvs, new_normals);
}