#include "Input.h"

#include "renderer/Window.h"

#include <iostream>

void Input::HandleInput(Window* window)
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	const double currentTime = glfwGetTime();
	const float deltaTime = static_cast<float>(currentTime - lastTime);

	// Get Mouse position
	double xPos, yPos;
	glfwGetCursorPos(window->m_window, &xPos, &yPos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window->m_window, 1600.0 / 2, 900.0 / 2);

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
	if (glfwGetKey(window->m_window, GLFW_KEY_W) == GLFW_PRESS)
		m_position += direction * deltaTime * m_speed;
	if (glfwGetKey(window->m_window, GLFW_KEY_S) == GLFW_PRESS)
		m_position -= direction * deltaTime * m_speed;
	if (glfwGetKey(window->m_window, GLFW_KEY_D) == GLFW_PRESS)
		m_position += right * deltaTime * m_speed;
	if (glfwGetKey(window->m_window, GLFW_KEY_A) == GLFW_PRESS)
		m_position -= right * deltaTime * m_speed;
	if (glfwGetKey(window->m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window->m_window, 1);

	window->m_camera.SetProjectionMatrix(glm::perspective(
		glm::radians(window->m_camera.GetFieldOfView()),
		4.0f / 3.0f,
		0.1f,
		100.0f
	));
	window->m_camera.SetViewMatrix(glm::lookAt(
		m_position,
		m_position + direction,
		up
	));

	lastTime = currentTime;
}

