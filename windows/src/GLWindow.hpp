// @file GLWindow.hpp
// @brief Gerencia a janela e o contexto OpenGL usando GLFW para a plataforma Windows
// @details Esta classe encapsula a criação e gerenciamento de uma janela usando GLFW, incluindo a configuração do contexto OpenGL e o loop de eventos. Ela herda de GLWindowHandler para fornecer uma interface consistente para lidar com entradas, troca de buffers e eventos, permitindo que a lógica de renderização seja implementada em classes derivadas.
//
// @author Natanael de Sousa
// @date 2026-05-23
// ------------------------------------------------------------------------------------------------------
//
#pragma once

// Engine headers
#include "Engine/GLWindowHandler.hpp"

// Gerencia a janela e o contexto OpenGL usando GLFW para a plataforma Windows
class GLWindow : public GLWindowHandler
{
private:
    GLFWwindow *window; // Ponteiro para a janela GLFW Windows

public:
    GLWindow(int width, int height, const char *title, GLFWmonitor *monitor = NULL, GLFWwindow *share = NULL); // Construtor que inicializa GLFW e cria uma janela
    ~GLWindow();                                                                                               // Destrutor que limpa os recursos da janela e termina GLFW
    virtual void InputsHandler();                                                                              // Função para lidar com entradas do usuário (pode ser sobrescrita para adicionar lógica de entrada personalizada)
    void SwapBuffers() const;                                                                                  // Função para trocar os buffers da janela
    void PollEvents() const;                                                                                   // Função para processar eventos da janela
    bool ShouldClose() const;                                                                                  // Função para verificar se a janela deve ser fechada
    virtual void RunEventLoop();                                                                               // Função para executar o loop de eventos da janela padrão (pode ser sobrescrita para adicionar lógica de renderização)
    void Destroy();                                                                                            // Função para destruir a janela e limpar os recursos associados
    GLFWwindow *getWindow() const;                                                                             // Função para obter o ponteiro da janela (se necessário para outras operações)
    void GetScreenSize(int &width, int &height) const;                                                         // Função para obeter o ponteiro do tamanho da tela da janela

    // Desabilita a cópia do GLWindow para evitar problemas de gerenciamento de recursos
    GLWindow(const GLWindow &) = delete;
    GLWindow &operator=(const GLWindow &) = delete;
};