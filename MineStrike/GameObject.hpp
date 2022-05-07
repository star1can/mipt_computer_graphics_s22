#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.hpp"

class GameObject
{
protected:
    Model model;
    glm::vec3 pos;
    glm::vec3 dir;
    float scale;

public:
    GameObject(const Model &model_, glm::vec3 pos_, glm::vec3 dir_, float scale_ = 1.0f) : model(model_), pos(pos_), dir(dir_), scale(scale_) {}

    Model GetModel();
    
    glm::vec3 GetPos();

    glm::vec3 GetDir();
    

    glm::mat4 GetModelMatrix();
};