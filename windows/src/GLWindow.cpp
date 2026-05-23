// @file GLWindow.cpp
// @brief Gerencia a janela e o contexto OpenGL usando GLFW para a plataforma Windows
// @details Esta classe encapsula a criação e gerenciamento de uma janela usando GLFW, incluindo a configuração do contexto OpenGL e o loop de eventos. Ela herda de GLWindowHandler para fornecer uma interface consistente para lidar com entradas, troca de buffers e eventos, permitindo que a lógica de renderização seja implementada em classes derivadas.
//
// @author Natanael de Sousa
// @date 2026-05-23
// ------------------------------------------------------------------------------------------------------
//

// OpenGL e GLFW headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Engine headers
#include "GLWindow.hpp"

// C++ standard library
#include <iostream>

// Callback para redimensionamento da janela, que atualiza o viewport do OpenGL para corresponder ao novo tamanho da janela
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Atualiza o OpenGL com o novo tamanho da janela
    glViewport(0, 0, width, height);
}

// Construtor que inicializa GLFW e cria uma janela
GLWindow::GLWindow(int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share)
{
    // Inicializa GLFW
    if (!glfwInit())
    {
        std::cerr << "Falha ao inicializar GLFW\n";
        throw std::runtime_error("Falha ao inicializar GLFW");
    }

    // Configura as dicas para a criação do contexto OpenGL (versão 3.3 core profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // Define a versão principal do OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // Define a versão menor do OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Define o perfil do OpenGL para core

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Necessário para rodar em macOS
#endif

    // Cria uma janela GLFW
    window = glfwCreateWindow(width, height, title, monitor, share);
    if (!window)
    {
        std::cerr << "Falha ao criar janela GLFW\n";
        glfwTerminate();
        throw std::runtime_error("Falha ao criar janela GLFW");
    }

    // Torna o contexto OpenGL da janela atual
    glfwMakeContextCurrent(window);

    // Inicializa GLAD para carregar as funções do OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Falha ao inicializar GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Falha ao inicializar GLAD");
    }

    // Configura o viewport e o callback para redimensionamento da janela
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Configura o callback para redimensionamento da janela
    std::cout << "Inicializando Execucao..." << std::endl;
}

// Destrutor que limpa os recursos da janela e termina GLFW
GLWindow::~GLWindow()
{
    Destroy(); // Garante que os recursos sejam limpos e GLFW seja terminado
}

void GLWindow::InputsHandler()
{
    // Teste de entrada: fecha a janela quando a tecla ESC for pressionada
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        std::cout << "Finalizando Execucao..." << std::endl;
        glfwSetWindowShouldClose(window, true);
    }
}

// Função para trocar os buffers da janela
void GLWindow::SwapBuffers() const
{
    glfwSwapBuffers(window);
}

// Função para processar eventos da janela
void GLWindow::PollEvents() const
{
    glfwPollEvents();
}

// Função para verificar se a janela deve ser fechada
bool GLWindow::ShouldClose() const
{
    return glfwWindowShouldClose(window);
}

// Função para executar o loop de eventos da janela padrão (pode ser sobrescrita para adicionar lógica de renderização)
void GLWindow::RunEventLoop()
{
    // Loop principal de eventos da janela
    while (!ShouldClose())
    {
        InputsHandler(); // Lida com entradas do usuário
        SwapBuffers();
        PollEvents();
    }
}

// Função para destruir a janela e limpar os recursos associados
void GLWindow::Destroy()
{
    if (window) // Verifica se a janela foi criada antes de tentar destruí-la
        glfwDestroyWindow(window);
    glfwTerminate(); // Garante que GLFW seja terminado mesmo que a janela não tenha sido criada com sucesso
}

// Função para obter o ponteiro da janela (se necessário para outras operações)
GLFWwindow *GLWindow::getWindow() const
{
    return window;
}