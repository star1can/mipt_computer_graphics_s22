#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.hpp"


class Enemy {
private:
    Model model;
    
    glm::vec3 pos;
    glm::vec3 dir;

public:
    // Enemy(const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals, const std::vector<glm::vec2> &uvs) : vertices_(vertices), normals_(normals), uvs_(uvs) {
    //     set_position();
    // }

    Enemy(const Model &model_, glm::vec3 pos, glm::vec3 dir);
};