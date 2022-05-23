#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.hpp"

class Camera
{
private:
    const float pi = 3.14159265359;
    Window window;

    double last_update_time;

    float movement_speed;
    float mouse_speed;
    float fov_speed;

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 dir = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    GLfloat pitch = 0.0f;
    GLfloat yaw = 0.0f;
    GLfloat roll = 0.0f;

    GLfloat fov = 45.0f;

    GLfloat limit_angle = 89.0f;

    void UpdateFromKeyboard(GLfloat deltaTime);
    void UpdateFromMouse(GLfloat deltaTime);

public:
    Camera(const Window &window_,
           float movement_speed_ = 4.0f,
           float mouse_speed_ = 0.0051f,
           float fov_speed_ = 1.0f);

    void Update();

    glm::vec3 GetPos();
    glm::vec3 GetDir();
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    float GetMovementSpeed();
    float GetMouseSpeed();
    void SetMovementSpeed(float new_speed);
    void SetMouseSpeed(float new_speed);
};