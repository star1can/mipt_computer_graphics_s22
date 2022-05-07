#include "GameObject.hpp"

Model GameObject::GetModel()
{
    return model;
}
glm::vec3 GameObject::GetPos()
{
    return pos;
}

glm::vec3 GameObject::GetDir()
{
    return dir;
}

glm::mat4 GameObject::GetModelMatrix()
{
    glm::mat4 translate_mat = glm::translate(glm::mat4(), pos);
    glm::mat4 rotate_mat = glm::rotate(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 scale_mat = glm::scale(glm::mat4(), glm::vec3(scale));

    return translate_mat * scale_mat * rotate_mat;
}