// @file main.cpp
// @brief Gerencia a janela e o contexto OpenGL usando EGL para a plataforma Android
// @details Esta classe encapsula a criação e gerenciamento de uma janela usando EGL, incluindo a configuração do contexto OpenGL ES e o loop de eventos. Ela herda de GLWindowHandler para fornecer uma interface consistente para lidar com entradas, troca de buffers e eventos, permitindo que a lógica de renderização seja implementada em classes derivadas.
//
// @author Natanael de Sousa
// @date 2026-05-24
// ------------------------------------------------------------------------------------------------------
//
#pragma once

// Android Tools - https://developer.android.com/studio
// Android NDK - https://developer.android.com/ndk
#include <android_native_app_glue.h>

// EGL e OpenGLES
#include <EGL/egl.h>
#include <GLES3/gl3.h>

// Engine headers
#include "Engine/GLWindowHandler.hpp"

// Classe GLAndroid que herda de GLWindowHandler e implementa os métodos virtuais para lidar com a janela e eventos no Android
class GLAndroid : public GLWindowHandler
{
protected:
    static EGLDisplay display; // Variável estática para armazenar o display EGL compartilhado entre todas as instâncias de GLAndroid
    static EGLSurface surface; // Variável estática para armazenar a superfície EGL compartilhada entre todas as instâncias de GLAndroid
    static EGLContext context; // Variável estática para armazenar o contexto EGL compartilhado entre todas as instâncias de GLAndroid
    android_app *g_app;        // Variável estática para armazenar o ponteiro para a estrutura android_app compartilhada entre todas as instâncias de GLAndroid
public:
    GLAndroid(android_app *app, void (*app_cmd)(android_app *, int32_t) = nullptr); // Construtor padrão para inicializar a classe GLAndroid
    ~GLAndroid() = default;                                                         // Destrutor padrão para a classe GLAndroid, não é necessário implementar nada aqui, pois os recursos EGL serão limpos no método Destroy()
    void Init() const;                                                              // Método para inicializar a janela e os recursos necessários para renderização no Android
    virtual void InputsHandler() override;                                          // Implementação dos métodos virtuais da classe base GLWindowHandler
    void SwapBuffers() const override;                                              // Implementação do método para trocar os buffers da janela usando EGL
    void PollEvents() const override;                                               // Implementação do método para verificar e processar eventos no Android
    bool ShouldClose() const override;                                              // Implementação do método para verificar se a janela deve ser fechada
    virtual void RunEventLoop() override;                                           // Implementação do método para executar o loop principal de eventos no Android
    void Destroy() override;                                                        // Implementação do método para destruir a janela e limpar recursos
};