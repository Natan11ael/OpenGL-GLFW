// @file main.cpp
// @brief Gerencia a janela e o contexto OpenGL usando EGL para a plataforma Android
// @details Esta classe encapsula a criação e gerenciamento de uma janela usando EGL, incluindo a configuração do contexto OpenGL ES e o loop de eventos. Ela herda de GLWindowHandler para fornecer uma interface consistente para lidar com entradas, troca de buffers e eventos, permitindo que a lógica de renderização seja implementada em classes derivadas.
//
// @author Natanael de Sousa
// @date 2026-05-24
// ------------------------------------------------------------------------------------------------------
//

// Engine headers
#include "GLAndroid.hpp"
#include "Engine/GLLog.hpp"

// Declaração das variáveis estáticas para armazenar o display, a superfície e o contexto EGL compartilhados entre todas as instâncias de GLAndroid
EGLDisplay GLAndroid::display = EGL_NO_DISPLAY; // Inicializa o display EGL como não disponível
EGLSurface GLAndroid::surface = EGL_NO_SURFACE; // Inicializa a superfície EGL como não disponível
EGLContext GLAndroid::context = EGL_NO_CONTEXT; // Inicializa o contexto EGL como não disponível

// Construtor padrão para inicializar a classe GLAndroid
GLAndroid::GLAndroid(android_app *app, void (*app_cmd)(android_app *, int32_t))
{
    g_app = app;               // Armazena o ponteiro para a estrutura android_app na variável estática g_app para que possa ser acessado nos métodos da classe
    g_app->userData = this;    // Armazena o ponteiro para a instância atual de GLAndroid na estrutura android_app para que possa ser acessado nos callbacks
    g_app->onAppCmd = app_cmd; // Configura o callback para lidar com os comandos do aplicativo
};

// Método para inicializar a janela e os recursos necessários para renderização no Android
void GLAndroid::Init() const
{
    EGLConfig cfg; // Variável para armazenar a configuração EGL escolhida
    EGLint n;      // Variável para armazenar o número de configurações EGL disponíveis

    // Configura o display EGL para usar a janela nativa do Android e inicializa o contexto EGL para renderização OpenGL ES 3.0
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY)
        LOGE("Unable to get EGL display");

    if (!eglInitialize(display, 0, 0)) // Inicializa o display EGL e verifica se a inicialização foi bem-sucedida, caso contrário, exibe um erro e aborta o aplicativo
        LOGE("Unable to initialize EGL");

    // Configura os atributos para escolher um formato de configuração EGL que suporte OpenGL ES 3.0
    EGLint attr[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT, EGL_NONE};
    if (!eglChooseConfig(display, attr, &cfg, 1, &n))
        LOGE("Unable to choose EGL config");

    if (n <= 0) // Verifica se alguma configuração EGL adequada foi encontrada, caso contrário, exibe um erro e aborta o aplicativo
        LOGE("No suitable EGL configs found");

    // Cria o contexto EGL para OpenGL ES 3.0
    EGLint ctxAttr[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    context = eglCreateContext(display, cfg, EGL_NO_CONTEXT, ctxAttr);
    if (context == EGL_NO_CONTEXT)
        LOGE("Unable to create EGL context");

    // Cria a superfície EGL usando a janela nativa do Android e a configuração escolhida
    surface = eglCreateWindowSurface(display, cfg, (EGLNativeWindowType)g_app->window, nullptr);
    if (surface == EGL_NO_SURFACE)
        LOGE("Unable to create EGL surface");

    // Torna o contexto EGL atual para renderização usando a superfície criada
    if (!eglMakeCurrent(display, surface, surface, context))
        LOGE("Unable to make EGL context current");

    // Define a cor de limpeza para preto com opacidade total
    glClearColor(0, 0, 0, 1);
}

// Implementação dos métodos virtuais da classe base GLWindowHandler
void GLAndroid::InputsHandler()
{
    // Implementação para lidar com entradas do usuário no Android
}

// Implementação do método para trocar os buffers da janela usando EGL
void GLAndroid::SwapBuffers() const
{
    eglSwapBuffers(display, surface); // Troca os buffers da janela para exibir o conteúdo renderizado
}

// Implementação do método para verificar e processar eventos no Android
void GLAndroid::PollEvents() const
{
    // Variáveis para armazenar os eventos e a fonte de eventos do Android
    int events;
    android_poll_source *source;

    // Loop para verificar e processar eventos do Android usando ALooper_pollAll, que bloqueia até que um evento esteja disponível ou o tempo limite seja atingido. O callback associado ao evento é chamado para processar o evento.
    while (ALooper_pollAll(0, nullptr, &events, (void **)&source) >= 0)
    {
        if (source)
            source->process(g_app, source);
    }
}

// Implementação do método para verificar se a janela deve ser fechada
bool GLAndroid::ShouldClose() const
{
    return g_app->destroyRequested != 0;
}

// Implementação do método para executar o loop principal de eventos no Android
void GLAndroid::RunEventLoop()
{
    // Loop principal de eventos do Android para processar entradas e renderizar continuamente até que a janela seja fechada
    while (!ShouldClose())
    {
        PollEvents(); // Verifica e processa eventos do Android

        if (display == EGL_NO_DISPLAY) // Se o display EGL não estiver disponível, saia do loop para evitar erros de renderização
            continue;

        // Renderiza o conteúdo da janela (aqui você pode adicionar suas chamadas de renderização OpenGL)
        glClear(GL_COLOR_BUFFER_BIT);
        SwapBuffers(); // Troca os buffers para exibir o conteúdo renderizado
    }
}

// Implementação do método para destruir a janela e limpar recursos
void GLAndroid::Destroy()
{
    // Limpa recursos EGL
    if (display != EGL_NO_DISPLAY)
    {
        // Desfaz a associação do contexto e da superfície EGL antes de destruí-los
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (context != EGL_NO_CONTEXT)
            eglDestroyContext(display, context); // Destrói o contexto EGL

        if (surface != EGL_NO_SURFACE)
            eglDestroySurface(display, surface); // Destrói a superfície EGL

        eglTerminate(display); // Termina a conexão com o display EGL
    }

    // Redefine as variáveis estáticas para indicar que os recursos foram limpos
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
}

// 
void GLAndroid::GetScreenSize(int &width, int &height) const
{
    eglQuerySurface(display, surface, EGL_WIDTH, &width);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height);
}