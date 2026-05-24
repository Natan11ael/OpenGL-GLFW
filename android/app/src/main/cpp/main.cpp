// @file main.cpp
// @brief Template de programa para Android usando OpenGL ES e EGL, com uma classe GLAndroid que gerencia a janela e os eventos no Android
// @details Este programa serve como um ponto de partida para criar aplicativos Android que utilizam OpenGL ES para renderização. Ele inclui uma classe GLAndroid que herda de GLWindowHandler e implementa os métodos necessários para lidar com a janela, eventos e renderização no Android usando EGL. O programa também define um loop principal de eventos para processar entradas e renderizar continuamente até que a janela seja fechada.
//
// @author Natanael de Sousa
// @date 2026-05-24
// @version 0.0.0 Pre-alpha (Desenvolvimento Ativo)
//
// @copyright Copyright (c) 2026 - Todos os direitos reservados.
// ------------------------------------------------------------------------------------------------------
//
#ifdef __cplusplus
extern "C" void app_dummy();
#endif
//

// Engine headers
#include "GLAndroid.hpp"
#include "Engine/GLImage.hpp"
#include "Engine/GLShader.hpp"
#include "Engine/GLMesh.hpp"
#include "Engine/GLLog.hpp"

// Classe MyProgram que herda de GLAndroid e implementa o método RunEventLoop para executar o loop principal de eventos no Android
class MyProgram : public GLAndroid
{
public:
    MyProgram(android_app *app, void (*app_cmd)(android_app *, int32_t) = nullptr) : GLAndroid(app, app_cmd) {} // Construtor que chama o construtor da classe base GLAndroid para inicializar a janela e os eventos no Android

    // Declaração do método para executar o loop principal de eventos, que será implementado posteriormente
    void RunEventLoop() override
    {
        // Configura o dataset para o loop de eventos
        GLImage image;
        GLShader shader;
        GLMesh mesh;
        bool loaded = false;

        // Loop principal de eventos do Android para processar entradas e renderizar continuamente até que a janela seja fechada
        while (!ShouldClose())
        {
            PollEvents(); // Verifica e processa eventos do Android

            // Se o display EGL não estiver disponível, saia do loop para evitar erros de renderização
            if (display == EGL_NO_DISPLAY) 
            {
                loaded = false;
                continue;
            }

            // 
            if (!loaded)
            {
                // Carrega os shaders usando a classe GLShader.
                if (!shader.Load("Shaders/point.vs.glsl", "Shaders/point.fs.glsl", g_app->activity->assetManager))
                    LOGE("Falha ao carregar shaders. Verifique os caminhos.");

                // Cria um quadrado cobrindo toda a tela e obtém o ID do VAO para renderização
                float vertices[] = {
                    -1.0f, -1.0f, 0.0f, 0.0f,
                    1.0f, -1.0f, 1.0f, 0.0f,
                    1.0f, 1.0f, 1.0f, 1.0f,
                    -1.0f, 1.0f, 0.0f, 1.0f};
                unsigned int indices[] = {0, 1, 2, 2, 3, 0};
                std::vector<GLVertexAttribute> attributes = {
                    {0, 2, GL_FLOAT}, // Atributo de posição (index 0, 2 componentes, tipo float)
                    {1, 2, GL_FLOAT}  // Atributo de coordenadas de textura (index 1, 2 componentes, tipo float)
                };

                // Carrega os dados de vértice e índice para o mesh e configura os buffers
                if (!mesh.Load(vertices, sizeof(vertices), indices, sizeof(indices), attributes))
                    LOGE("Falha ao carregar mesh.");

                // Carrega a textura usando a classe GLImage.
                if (!image.Load("Textures/image.png", g_app->activity->assetManager)) // Carrega a textura a partir do arquivo usando stb_image e cria uma textura OpenGL
                    LOGE("Falha ao carregar imagem. Verifique o caminho.");

                loaded = true;
            }

            // Renderiza o conteúdo da janela (aqui você pode adicionar suas chamadas de renderização OpenGL)
            glClear(GL_COLOR_BUFFER_BIT);

            //
            shader.Use();
            glUniform1i(glGetUniformLocation(shader.Get(), "uTexture"), 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, image.Get());
            mesh.Draw();

            SwapBuffers(); // Troca os buffers para exibir o conteúdo renderizado
        }

        shader.Delete(); // Deleta o programa de shader para liberar os recursos associados
    }
};

// Handle app commands (like window creation/destruction)
static void handle_cmd(android_app *app, int32_t cmd)
{
    auto *self = static_cast<GLAndroid *>(app->userData);

    switch (cmd)
    {
    case APP_CMD_INIT_WINDOW:
        if (app->window)
        {
            self->Init(); // Inicializa a janela e os recursos necessários para renderização no Android
        }
        break;

    case APP_CMD_TERM_WINDOW:
        self->Destroy(); // Destrói a janela e os recursos necessários para renderização no Android
        break;
    }
}

// Função principal do aplicativo Android
extern "C" void android_main(struct android_app *app)
{
    // app_dummy(); // Função dummy para evitar que o linker remova a função android_main | comentado quando nao for necessário
    MyProgram myProgram(app, handle_cmd); // Cria uma instância da classe MyProgram para gerenciar a janela e os eventos no Android
    myProgram.RunEventLoop();             // Executa o loop principal de eventos para processar entradas e renderizar continuamente até que a janela seja fechada
}