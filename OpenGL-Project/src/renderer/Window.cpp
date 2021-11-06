#include "Window.h"

#include <stdexcept>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "entity/Camera.h"
#include "input/Input.h"

Window::Window() = default;
Window::~Window() = default;

void Window::Initialize()
{
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    m_window = glfwCreateWindow(1600, 900, "OpenGL-Project", nullptr, nullptr);
    if (!m_window)
    {
        // Destroy the cursor, window etc.
        glfwTerminate();

        throw std::runtime_error("Failed to create GLFW Window");
    }

    glfwMakeContextCurrent(m_window);

    const uint32_t errorCode = glewInit();
    if (errorCode != GLEW_OK)
    {
        std::cout << "Failed to init GLEW: " << errorCode << std::endl;
        glfwTerminate();

        return;
    }

    InitializeImGui();

    m_renderer = new Renderer();
    m_renderer->Initialize();

    // Set initial camera position
    m_renderer->m_camera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
}

void Window::RunWindowLoop()
{
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(m_window, 1600.0 / 2, 900.0 / 2);

    Entity* entity = new Entity();
    entity->LoadModel(PROJECT_DIR("res/model/model.bin"));

    while (!glfwWindowShouldClose(m_window))
    {
        // Update frame times
        const double now = glfwGetTime();
        const double deltaTime = now - m_elapsedTime;
        m_elapsedTime = now;

        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Update the renderer to dequeue any enqueued draw operations
        m_renderer->Update(deltaTime);
        m_renderer->Draw(entity);

        // Update the Input for camera movement :eyes:
        Input::GetInstance().HandleInput(this);

        // Draw any GUI
        DrawImGui();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    delete entity;
}

void Window::InitializeImGui()
{
    if (!m_window)
        throw std::runtime_error("Something went wrong while initializing imgui, m_window is nullptr");

    // Create ImGui Context
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(reinterpret_cast<const char*>(glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS)));
    ImGui::StyleColorsDark();
}

void Window::DrawImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ImGui::Begin("Application"))
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        const glm::vec3& position = m_renderer->m_camera->GetPosition();
        ImGui::Text("Position X: %f Y: %f, Z: %f", position.x, position.y, position.z);

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::Shutdown()
{
    // Destroy the ImGui context
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Destroy current window and all resources
    glfwDestroyWindow(m_window);
    glfwTerminate();

    // Delete any pointers
    delete m_renderer;
}
