#ifndef INPUT_H__
#define INPUT_H__

class Window;

class Input
{
public:
    static Input& GetInstance()
    {
        static Input instance;
        return instance;
    }

    Input();

    void HandleInput(const Window* window);

private:
    double m_lastTime = 0.0f;

    float m_horizontalAngle = 3.14f;
    float m_verticalAngle = 0.0f;

    float m_speed = 3.0f;
    float m_mouseSpeed = 0.005f;
};

#endif // INPUT_H__
