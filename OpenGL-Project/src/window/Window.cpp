#include "Window.h"

#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>
#include <iostream>

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

    const int errorCode = glewInit();
    if (errorCode != GLEW_OK)
    {
        std::cout << "Failed to init GLEW: " << errorCode << std::endl;
        glfwTerminate();

        exit(-1);
    }

    InitializeImGui();
}

void Window::RunWindowLoop()
{
    Shader shader;
    if (!shader.LoadShader(PROJECT_DIR("shaders/vertex.vert"), PROJECT_DIR("shaders/fragment.frag")))
        return;

    constexpr float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    const glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1600.0f / 900.0f, 0.1f, 100.0f);
    const glm::mat4 view = glm::lookAt(
        glm::vec3(4, 3, 3),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );

    const glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = projection * view * model;

    while (!glfwWindowShouldClose(m_window))
    {
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        DrawImGui();

        // Bind the shader
        shader.Bind();

        const int32_t matrixId = shader.GetUniformLocation("MVP");
        glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Window::InitializeImGui()
{
    if (!m_window)
        throw std::runtime_error("Something went wrong while initializing imgui, m_window is nullptr");

    // Create ImGui Context
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
    ImGui::StyleColorsDark();
}

void Window::DrawImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Application");

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();

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
}
