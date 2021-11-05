#ifndef INPUT_H__
#define INPUT_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Window;

class Input
{
public:
    static Input& GetInstance()
    {
        static Input instance;
        return instance;
    }

    void HandleInput(Window* window);

    glm::vec3 const& GetPosition() const { return m_position; }

private:
    glm::vec3 m_position = glm::vec3(0, 0, 5);

    float m_horizontalAngle = 3.14f;
    float m_verticalAngle = 0.0f;

    float m_speed = 3.0f;
    float m_mouseSpeed = 0.005f;
};

#endif // INPUT_H__
