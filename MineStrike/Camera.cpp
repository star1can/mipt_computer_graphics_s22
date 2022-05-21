#include "Camera.hpp"
#include "Window.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const Window &window_,
			   float movement_speed_,
			   float mouse_speed_,
			   float fov_speed_) : window(window_),
								   movement_speed(movement_speed_),
								   mouse_speed(mouse_speed_),
								   fov_speed(fov_speed_)
{
	last_update_time = glfwGetTime();

	glm::vec2 window_center = glm::vec2(window.GetWidth() / 2.0f, window.GetHeight() / 2.0f);
	glfwSetCursorPos(window.GetWindow(), window_center.x, window_center.y);

	// Turn on unlimited rotation
	glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
};

void Camera::UpdateFromKeyboard(GLfloat delta_time)
{
	GLfloat sensivity = movement_speed * delta_time;
	glm::vec3 right = glm::normalize(glm::cross(up, dir));
	glm::vec3 front = glm::normalize(glm::cross(up, right));

	if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		pos -= sensivity * front;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		pos += sensivity * front;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		pos += sensivity * right;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		pos -= sensivity * right;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		pos += sensivity * up;
	}
	if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		pos -= sensivity * up;
	}
}

void Camera::UpdateFromMouse(GLfloat delta_time)
{
	glm::vec2 window_center = glm::vec2(window.GetWidth() / 2.0f, window.GetHeight() / 2.0f);
	GLfloat sensitivity = mouse_speed * delta_time;

	double x, y;
	glfwGetCursorPos(window.GetWindow(), &x, &y);
	glfwSetCursorPos(window.GetWindow(), window_center.x, window_center.y);

	yaw -= (window_center.x - x) * sensitivity;
	pitch += (window_center.y - y) * sensitivity;

	if (pitch * 180.0f / pi >= limit_angle)
	{
		pitch = pi * limit_angle / 180.0f;
	}
	if (pitch * 180.0f / pi <= -limit_angle)
	{
		pitch = pi * -limit_angle / 180.0f;
	}

	dir = glm::vec3(cos(pitch) * cos(yaw),
					sin(pitch),
					cos(pitch) * sin(yaw));
}

void Camera::Update()
{
	double delta_time = glfwGetTime() - last_update_time;

	UpdateFromKeyboard(delta_time);
	UpdateFromMouse(delta_time);

	last_update_time = glfwGetTime();
}

glm::vec3 Camera::GetPos()
{
	return pos;
}

glm::vec3 Camera::GetDir()
{
	return dir;
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(
		pos,	   // camera position
		pos + dir, // position to look at
		up		   // tells where UP is. Defines rotation of camera over the direction of view
	);
}

// Homogeneous transformations
glm::mat4 Camera::GetProjectionMatrix()
{
	glm::mat4 projection_matrix = glm::perspective(
		glm::radians(fov),									  // FOV (field of view)
		float(window.GetWidth()) / float(window.GetHeight()), // ratio of rectangle; determines camera's frustum longitudinal shape
		0.1f,												  // determines distance to the near face of camera's frustum
		1000.0f												  // determines distance to the far side of camera's frustum
	);

	return projection_matrix;
}