#include "Projectile.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Projectile::Projectile(const Model &model_,
                       glm::vec3 pos_,
                       glm::vec3 dir_,
                       float speed_) : GameObject(model_, pos_, dir_),
                                       speed(speed_)
{
    last_update_time = glfwGetTime();
};

void Projectile::UpdatePos()
{
    float delta_time = glfwGetTime() - last_update_time;

    pos += dir * speed * delta_time;

    last_update_time = glfwGetTime();
}