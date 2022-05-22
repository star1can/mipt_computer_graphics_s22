#include "Enemy.hpp"

Enemy::Enemy(const Model &model_,
             glm::vec3 pos_,
             glm::vec3 dir_) : model(model_),
                               pos(pos_),
                               dir(dir_){};