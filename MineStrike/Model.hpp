#pragma once



// #include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
// #include <glm/glm.hpp>

// #include <common/shader.hpp>
// #include <common/texture.hpp>
// #include <common/objloader.hpp>

// #include "Generate_VBO_VAO.hpp"


// Идейно модель не должна выпонять никаких функций, она лишь хранит свои свойства
// Впорос в том как стоит заполнять эти поля
// нужно штуки 3 функции чтобы их заполнить и либо они идут в отдельные файлы либо в  main
// либо в методы модели чтоб можно было вызвать конструктор модели с кучей параметров

// Вопрос: а точно класс окна должен заниматься отрисовкой моделей? там точно не нужно ещё одного уровня абстракции ?
// Ответ: по идее нет. все те функции что надо вызвать самые базовые и
// единственное что можно было бы сделать это класс настройщика выставляющего кучу параметров - что фигово звучит и пишется тем более
//
// Класс шейдера лишник. достаточно хранить shader program и к ней когда нужно сделать дополнительные действия обращаться
//
// Делать разделение на класс отрисовки и игровой логики - перебор - слишком долго

// struct Model
// {
//     GLuint textureID;
//     GLuint VBO;
//     GLuint VAO; //stores setting we set in for VBO
//     GLuint shader_program;
//     const GLfloat *vertex_array;
//     GLuint vertex_count;
// };

struct Model
{

    GLuint VAO;
    GLuint texture;
    GLuint shader_program;
    GLuint glsl_mvp_matrix;
    GLuint glsl_texture;
    GLuint triangles_count;

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
};