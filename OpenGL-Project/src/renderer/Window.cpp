#include "Window.h"

#include <stdexcept>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
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

        std::exit(-1);
    }

    InitializeImGui();
}

void Window::RunWindowLoop()
{
    Shader shader;
    if (!shader.LoadShader(PROJECT_DIR("res/shaders/vertex.vert"), PROJECT_DIR("res/shaders/fragment.frag")))
        return;

    constexpr float vertices[] = {
        -1.0f, -1.0f, -1.0f, // triangle 1 : begin
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f, // triangle 1 : end
         1.0f,  1.0f, -1.0f, // triangle 2 : begin
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f, // triangle 2 : end
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    constexpr float colors[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };

    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    uint32_t vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Colors
    uint32_t colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

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

    while (!glfwWindowShouldClose(m_window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        DrawImGui();
        Input::GetInstance().HandleInput(this);

        // Bind the shader
        shader.Bind();

        // Pass the camera position to the shader
        const int32_t matrixId = shader.GetUniformLocation("u_modelMatrix");
        glm::mat4 mvp = m_camera.GetProjectionMatrix() * m_camera.GetViewMatrix() * glm::mat4(1.0f);
        glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

        // Enable 0 location of vertex shader
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Enable 1 location of vertex shader
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &colorBuffer);
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

        const glm::vec3 position = Input::GetInstance().GetPosition();
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
}
