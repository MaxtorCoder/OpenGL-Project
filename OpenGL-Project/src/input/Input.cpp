#include "Input.h"

#include "renderer/Window.h"
#include "entity/Camera.h"

#include <glm/glm.hpp>

Input::Input() : m_lastTime(glfwGetTime()) {}

void Input::HandleInput(const Window* window)
{
	GLFWwindow* glfwWindow = window->GetWindow();

	// Compute time difference between current and last frame
	const double currentTime = glfwGetTime();
	const float deltaTime = static_cast<float>(currentTime - m_lastTime);

	// Get Mouse position
	double xPos, yPos;
	glfwGetCursorPos(glfwWindow, &xPos, &yPos);

	// Reset mouse position for next frame
	glfwSetCursorPos(glfwWindow, 1600.0 / 2, 900.0 / 2);

	// Compute new orientation
	m_horizontalAngle += m_mouseSpeed * static_cast<float>(1600.0 / 2 - xPos);
	m_verticalAngle += m_mouseSpeed * static_cast<float>(900.0 / 2 - yPos);

	// Direction
	const glm::vec3 direction(
		cos(m_verticalAngle) * sin(m_horizontalAngle),
		sin(m_verticalAngle),
		cos(m_verticalAngle) * cos(m_horizontalAngle)
	);

	// Right vector
	const glm::vec3 right = glm::vec3(
		sin(m_horizontalAngle - 3.14f / 2.0f),
		0,
		cos(m_horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	const glm::vec3 up = glm::cross(right, direction);

	// Update position
	Camera* camera = window->GetRenderer()->m_camera;
	if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
		camera->SetPosition(camera->GetPosition() + direction * deltaTime * m_speed);
	if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		camera->SetPosition(camera->GetPosition() - direction * deltaTime * m_speed);
	if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
		camera->SetPosition(camera->GetPosition() + right * deltaTime * m_speed);
	if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
		camera->SetPosition(camera->GetPosition() - right * deltaTime * m_speed);

	if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glfwWindow, 1);

	camera->UpdateProjectionMatrix();
	camera->UpdateViewMatrix(direction, up);

	m_lastTime = currentTime;
}

