#ifndef RENDERER_H__
#define RENDERER_H__

#include <queue>
#include <vector>

class Camera;
class Entity;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Initialize();
    void EnqueueDrawEntity(Entity* entity);
    void Draw(const Entity* entity) const;

    void Update(float deltaTime);

public:
    Camera* m_camera = nullptr;

private:

    std::queue<Entity*> m_enqueuedEntities = {};
    std::vector<Entity*> m_activeEntities = {};

    uint32_t m_vertexBufferId;
    uint32_t m_uvBufferId;
    uint32_t m_vertexArrayId;

};

#endif // RENDERER_H__
