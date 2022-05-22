#pragma once



// #include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>


struct Model {

    GLuint VAO;
    GLuint texture;
    GLuint shader_program;
    GLuint glsl_mvp_matrix;
    GLuint glsl_texture;
    GLuint triangles_count;
    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec2> uvs_;

    Model(GLuint VAO,
          GLuint texture,
          GLuint shader_program,
          GLuint triangles_count,
          GLuint glsl_mvp_matrix,
          GLuint glsl_texture) : VAO(VAO),
                                 texture(texture),
                                 shader_program(shader_program),
                                 glsl_mvp_matrix(glsl_mvp_matrix),
                                 glsl_texture(glsl_texture),
                                 triangles_count(triangles_count) {}

    Model(const char *obj_file,
          const char *dds_file,
          const char *vertex_shader_file,
          const char *fragment_shader_file,
          const char *glsl_mvp_matrix_name = "MVP",
          const char *glsl_texture_name = "myTextureSampler");

    std::vector<glm::vec3> &GetVertices();

    std::vector<glm::vec3> &GetNormals();

    std::vector<glm::vec2> &GetUvs();

    void UpdateVAO(
            const std::vector<glm::vec3> &new_vertices,
            const std::vector<glm::vec2> &new_uvs,
            const std::vector<glm::vec3> &new_normals);
};