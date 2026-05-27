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

// OpenGL e GLFW headers
#include <glm/glm.hpp>
#include <GLES3/gl3.h>

#include "NanoVG/nanovg.h"
#define NANOVG_GLES3_IMPLEMENTATION
#include "NanoVG/nanovg_gl.h"

// Engine headers
#include "GLAndroid.hpp"
#include "Engine/GLImage.hpp"
#include "Engine/GLShader.hpp"
#include "Engine/GLMesh.hpp"
#include "Engine/GLLog.hpp"

// C++ 
#include <time.h>

// função helper
float GetTime()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (float)ts.tv_sec + (float)ts.tv_nsec / 1e9f;
}

// Classe MyProgram que herda de GLAndroid e implementa o método RunEventLoop para executar o loop principal de eventos no Android
class MyProgram : public GLAndroid
{
public:
    MyProgram(android_app *app, void (*app_cmd)(android_app *, int32_t) = nullptr) : GLAndroid(app, app_cmd) {} // Construtor que chama o construtor da classe base GLAndroid para inicializar a janela e os eventos no Android

    // Declaração do método para executar o loop principal de eventos, que será implementado posteriormente
    void RunEventLoop() override
    {
        // Configura o dataset para o loop de eventos
        NVGcontext *vg = nullptr;
        int nvg_sprite;
        int font = -1;
        GLShader s_metalball, s_sprite;
        GLMesh m_metalball, m_sprite;
        GLImage img_sprite;
        bool loaded = false;

        // Loop principal de eventos do Android para processar entradas e renderizar continuamente até que a janela seja fechada
        while (!ShouldClose())
        {
            PollEvents(); // Verifica e processa eventos do Android

            //
            int width, height;
            GetScreenSize(width, height);

            // Se o display EGL não estiver disponível, saia do loop para evitar erros de renderização
            if (display == EGL_NO_DISPLAY)
            {
                if (vg)
                {
                    nvgDeleteGLES3(vg);
                    vg = nullptr;
                }
                loaded = false;
                continue;
            }

            //
            if (!loaded)
            {
                //
                // Carrega os shaders usando a classe GLShader.
                if (!s_metalball.Load("Shaders/metalball.vs.glsl", "Shaders/metalball.fs.glsl", g_app->activity->assetManager)) // Carrega os shaders de vértice e fragmento a partir dos arquivos
                    LOGE("Falha ao carregar shaders.");

                // Cria um quadrado cobrindo toda a tela e obtém o ID do VAO para renderização
                float v_metalball[] = {-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};
                unsigned int id_metalball[] = {0, 1, 2, 2, 3, 0};
                std::vector<GLVertexAttribute> att_metalball = {
                    {0, 2, GL_FLOAT}, // Atributo de posição (index 0, 2 componentes, tipo float)
                };

                // Carrega os dados de vértice e índice para o mesh e configura os buffers
                if (!m_metalball.Load(v_metalball, sizeof(v_metalball), id_metalball, sizeof(id_metalball), att_metalball, GL_TRIANGLES))
                    LOGE("Falha ao carregar mesh.");

                //
                // Carrega os shaders usando a classe GLShader.
                if (!s_sprite.Load("Shaders/sprite.vs.glsl", "Shaders/sprite.fs.glsl", g_app->activity->assetManager)) // Carrega os shaders de vértice e fragmento a partir dos arquivos
                    LOGE("Falha ao carregar shaders.");

                // Cria um quadrado cobrindo toda a tela e obtém o ID do VAO para renderização
                float v_sprite[] = {-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f};
                unsigned int id_sprite[] = {0, 1, 2, 2, 3, 0};
                std::vector<GLVertexAttribute> att_sprite = {
                    {0, 2, GL_FLOAT}, // Atributo de posição (index 0, 2 componentes, tipo float)
                    {1, 2, GL_FLOAT}  // Atributo de coordenadas de textura (index 1, 2 componentes, tipo float)
                };

                // Carrega os dados de vértice e índice para o mesh e configura os buffers
                if (!m_sprite.Load(v_sprite, sizeof(v_sprite), id_sprite, sizeof(id_sprite), att_sprite, GL_TRIANGLES))
                    LOGE("Falha ao carregar mesh.");

                // Carrega a textura usando a classe GLImage.
                if (!img_sprite.Load("Textures/image.png", g_app->activity->assetManager)) // Carrega a textura a partir do arquivo usando stb_image e cria uma textura OpenGL
                    LOGE("Falha ao carregar imagem.");

                // NanoVG — inicializa após contexto estar pronto
                vg = nvgCreateGLES3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

                //
                nvg_sprite = nvglCreateImageFromHandleGLES3(vg, img_sprite.Get(), width, height, 0);

                // Fonte via AssetManager
                AAsset *font_asset = AAssetManager_open(g_app->activity->assetManager, "Fonts/ari-w9500.ttf", AASSET_MODE_BUFFER);
                if (font_asset)
                {
                    const void *font_data = AAsset_getBuffer(font_asset);
                    int font_size = AAsset_getLength(font_asset);
                    font = nvgCreateFontMem(vg, "sans", (unsigned char *)font_data, font_size, 0);
                    AAsset_close(font_asset);
                }

                loaded = true;
            }

            // Lógica de renderização
            glClear(GL_COLOR_BUFFER_BIT);

            nvgBeginFrame(vg, (float)width, (float)height, 1.0f);

            // Render Point
            nvgBeginPath(vg);
            nvgCircle(vg, 10.0f, 10.0f, 5.0f); // raio maior por conta do DPI
            nvgFillColor(vg, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
            nvgFill(vg);

            // Render Line
            nvgBeginPath(vg);
            nvgMoveTo(vg, 20.0f, 10.0f);
            nvgLineTo(vg, 40.0f, 10.0f);
            nvgStrokeColor(vg, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
            nvgStrokeWidth(vg, 2.0f);
            nvgStroke(vg);

            // Render Triangle
            nvgBeginPath(vg);
            nvgMoveTo(vg, 50.0f, 10.0f);
            nvgLineTo(vg, 50.0f, 30.0f);
            nvgLineTo(vg, 70.0f, 30.0f);
            nvgClosePath(vg);
            nvgFillColor(vg, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
            nvgFill(vg);
            nvgStrokeColor(vg, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
            nvgStrokeWidth(vg, 2.0f);
            nvgStroke(vg);

            // Render Rect
            nvgBeginPath(vg);
            nvgRect(vg, 80.0f, 10.0f, 20.0f, 20.0f);
            nvgFillColor(vg, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
            nvgFill(vg);
            nvgStrokeColor(vg, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
            nvgStrokeWidth(vg, 2.0f);
            nvgStroke(vg);

            // Render Ellipse
            nvgBeginPath(vg);
            nvgEllipse(vg, 140.0f, 20.0f, 25.0f, 15.0f);
            nvgFillColor(vg, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
            nvgFill(vg);
            nvgStrokeColor(vg, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
            nvgStrokeWidth(vg, 2.0f);
            nvgStroke(vg);

            // Render Sprite
            NVGpaint paint = nvgImagePattern(vg, 150.0f, 10.0f + 128.0f, 128.0f, -128.0f, 0.0f, nvg_sprite, 1.0f);
            nvgBeginPath(vg);
            nvgRect(vg, 180.0f, 10.0f, 128.0f, 128.0f);
            nvgFillPaint(vg, paint);
            nvgFill(vg);

            // Render Font
            if (font >= 0)
            {
                nvgFontFaceId(vg, font);
                nvgFontSize(vg, 24.0f);
                nvgFillColor(vg, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
                nvgText(vg, 20.0f, 100.0f, "Hello World", nullptr);
            }

            nvgEndFrame(vg);

            // Render Sprite
            s_sprite.Use();
            float aspect = (float)width / (float)height;
            glUniform2f(glGetUniformLocation(s_sprite.Get(), "u_pos"), 0.0125f, 0.75f);
            glUniform2f(glGetUniformLocation(s_sprite.Get(), "u_scale"), 0.21f, 0.21f);
            glUniform1f(glGetUniformLocation(s_sprite.Get(), "u_rotation"), glm::radians(0.0f));
            glUniform2f(glGetUniformLocation(s_sprite.Get(), "u_tex_scale"), 1.0f, 1.0f);
            glUniform4f(glGetUniformLocation(s_sprite.Get(), "u_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform1f(glGetUniformLocation(s_sprite.Get(), "u_time"), GetTime());
            glUniform1f(glGetUniformLocation(s_sprite.Get(), "u_intensity"), 0.5f);
            glUniform1f(glGetUniformLocation(s_sprite.Get(), "u_aspect"), aspect);
            glUniform1i(glGetUniformLocation(s_sprite.Get(), "u_texture"), 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, img_sprite.Get());
            m_sprite.Draw();

            // Render Metalball
            s_metalball.Use();
            const glm::vec2 m_center[]{{-0.04f, 0.01f}, {0.04f, 0.01f}, {0.0f, 0.04f}};
            const float m_radius[]{0.03f, 0.03f, 0.03f};
            const GLsizei count = sizeof(m_center) / sizeof(m_center[0]);
            glUniform2fv(glGetUniformLocation(s_metalball.Get(), "u_center"), count, &m_center[0].x);
            glUniform1fv(glGetUniformLocation(s_metalball.Get(), "u_radius"), count, m_radius);
            glUniform1i(glGetUniformLocation(s_metalball.Get(), "u_count"), count);
            glUniform4f(glGetUniformLocation(s_metalball.Get(), "u_fill_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform4f(glGetUniformLocation(s_metalball.Get(), "u_outline_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform1f(glGetUniformLocation(s_metalball.Get(), "u_threshold"), 0.8f);
            glUniform1f(glGetUniformLocation(s_metalball.Get(), "u_thickness"), 0.2f);
            glUniform1i(glGetUniformLocation(s_metalball.Get(), "u_outline"), true);
            m_metalball.Draw();

            SwapBuffers(); // Troca os buffers para exibir o conteúdo renderizado
        }

        // De
        s_metalball.Delete();
        m_metalball.Delete();
        s_sprite.Delete();
        m_sprite.Delete();
        img_sprite.Delete();
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