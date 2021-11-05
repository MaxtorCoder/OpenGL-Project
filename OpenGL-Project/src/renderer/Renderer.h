#ifndef RENDERER_H__
#define RENDERER_H__

class Camera;

class Renderer
{
public:
    Renderer();
    ~Renderer();

private:
    Camera* m_camera = nullptr;

};

#endif // RENDERER_H__
