#ifndef WINDOW_H__
#define WINDOW_H__

#include "common/Define.h"
#include "entity/Camera.h"

class Window
{
public:
    Window();
    ~Window();

    void Initialize();
    void RunWindowLoop();
    void Shutdown();

private:
    void InitializeImGui();
    void DrawImGui();

private:
    GLFWwindow* m_window = nullptr;
    Camera* m_camera = nullptr;
};

#endif // WINDOW_H__
