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

void GameObject::SetPos(glm::vec3 pos_)
{
    pos = pos_;
}

void GameObject::SetDir(glm::vec3 dir_)
{
    dir = dir_;
}

glm::mat4 GameObject::GetModelMatrix()
{
    glm::vec3 up = glm::vec3(1.0f, 1.0f, 0.0f);

    glm::mat4 translate_mat = glm::translate(glm::mat4(), pos);
    glm::mat4 look_at = glm::lookAt(
        pos,
        pos + dir,
        up
    );
    glm::mat4 scale_mat = glm::scale(glm::mat4(), glm::vec3(scale));

    return translate_mat * scale_mat;
}