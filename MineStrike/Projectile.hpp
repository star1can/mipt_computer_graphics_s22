#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Model.hpp"
#include "GameObject.hpp"

class Projectile : GameObject
{
    float speed;

    float last_update_time;

public:
    void UpdatePos();

    Projectile(const Model &model_, glm::vec3 pos_, glm::vec3 dir_, float speed_ = 1.0f);
};