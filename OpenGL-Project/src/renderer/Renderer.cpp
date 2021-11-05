#include "Renderer.h"

#include "common/Define.h"
#include "entity/Camera.h"

Renderer::Renderer() = default;
Renderer::~Renderer()
{
    delete m_camera;
}
