#ifndef WINDOW_H__
#define WINDOW_H__

#include "common/Define.h"
#include "Renderer.h"

class Window
{
public:
    Window();
    ~Window();

    void Initialize();
    void RunWindowLoop();
    void Shutdown();

    Renderer const* GetRenderer() const { return m_renderer; }
    GLFWwindow* GetWindow() const { return m_window; }

private:
    void InitializeImGui();
    void DrawImGui();

private:
    GLFWwindow* m_window = nullptr;
    Renderer* m_renderer = nullptr;

    double m_elapsedTime = 0.0;

};

#endif // WINDOW_H__
