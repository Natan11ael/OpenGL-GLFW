// @file GLWindowHandler.hpp
// @brief Gerencia a janela de contexto generico para a plataforma Windows ou Android
// @details Esta classe encapsula funções genericas para lidar com entradas, troca de buffers e eventos, permitindo que a lógica de renderização seja implementada em classes derivadas. Ela serve como uma interface base para diferentes implementações de janelas específicas para cada plataforma (como GLWindow para Windows e GLAndroid para Android).
//
// @author Natanael de Sousa
// @date 2026-05-23
// ------------------------------------------------------------------------------------------------------
//
#pragma once

class GLWindowHandler
{
public:
    GLWindowHandler() = default;          // Default constructor
    virtual ~GLWindowHandler() = default; // Virtual destructor for proper cleanup in derived classes
    virtual void InputsHandler() = 0;     // Pure virtual function to handle user inputs
    virtual void SwapBuffers() const = 0; // Pure virtual function to swap the buffers of the window
    virtual void PollEvents() const = 0;  // Pure virtual function to poll for and process events in the window
    virtual bool ShouldClose() const = 0; // Pure virtual function to check if the window should close
    virtual void RunEventLoop() = 0;      // Pure virtual function to run the main event loop of the window
    virtual void Destroy() = 0;           // Pure virtual function to destroy the window and clean up resources
};