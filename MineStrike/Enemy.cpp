#include "Enemy.hpp"
#include "Model.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

Enemy::Enemy(const Model &model_,
             glm::vec3 pos_,
             glm::vec3 dir_) : model(model_),
                               pos(pos_),
                               dir(dir_){};