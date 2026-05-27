// @file main.cpp
// @brief Template de programa para Windows usando OpenGL e GLFW, com uma classe GLWindow que gerencia a janela e os eventos no Windows
// @details Este programa serve como um ponto de partida para criar aplicativos Windows que utilizam OpenGL para renderização. Ele inclui uma classe GLWindow que gerencia a janela, o contexto OpenGL e os eventos usando GLFW. O programa também define um loop principal de eventos para processar entradas e renderizar continuamente até que a janela seja fechada.
//
// @author Natanael de Sousa
// @date 2026-05-22
// @version 0.0.0 Pre-alpha (Desenvolvimento Ativo)
//
// @copyright Copyright (c) 2026 - Todos os direitos reservados.
// ------------------------------------------------------------------------------------------------------
//

// OpenGL e GLFW headers
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "NanoVG/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "NanoVG/nanovg_gl.h"

// Engine headers
#include "GLWindow.hpp"
#include "Engine/GLShader.hpp"
#include "Engine/GLMesh.hpp"
#include "Engine/GLImage.hpp"
#include "Engine/GLLog.hpp"

// C++ standard library
#include <iostream>

// Classe que herda de WindowManager para implementar a lógica de renderização específica do programa
class MyProgram : public GLWindow
{
public:
    MyProgram() : GLWindow(800, 600, "OpenGL Example") {}

    // Função para executar o loop de eventos da janela
    void RunEventLoop()
    {
        // Configura o dataset para o loop de eventos
        NVGcontext *vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
        int nvg_sprite;
        int font;
        GLShader s_metalball, s_sprite;
        GLMesh m_metalball, m_sprite;
        GLImage img_sprite;
        
        bool loaded = false;

        // Loop principal de eventos da janela
        while (!ShouldClose())
        {
            // Lógica de entrada e renderização
            InputsHandler(); // Lida com entradas do usuário

            //
            int width, height;
            GetScreenSize(width, height);

            //
            if (!loaded)
            {

                //
                // Carrega os shaders usando a classe GLShader.
                if (!s_metalball.Load("Shaders/metalball.vs.glsl", "Shaders/metalball.fs.glsl")) // Carrega os shaders de vértice e fragmento a partir dos arquivos
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

                // Sprite Effect
                // Carrega os shaders usando a classe GLShader.
                if (!s_sprite.Load("Shaders/sprite.vs.glsl", "Shaders/sprite.fs.glsl")) // Carrega os shaders de vértice e fragmento a partir dos arquivos
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
                if (!img_sprite.Load("Textures/image.png")) // Carrega a textura a partir do arquivo usando stb_image e cria uma textura OpenGL
                    LOGE("Falha ao carregar imagem.");

                // Sprite Default
                nvg_sprite = nvglCreateImageFromHandleGL3(vg, img_sprite.Get(), width, height, 0);

                //
                font = nvgCreateFont(vg, "sans", "src/assets/Fonts/ari-w9500.ttf");

                loaded = true;
            }

            // Lógica de renderização
            glClear(GL_COLOR_BUFFER_BIT); // Limpa a tela para desenhar o próximo frame

            nvgBeginFrame(vg, (float)width, (float)height, 1.0f);

            // Render Point
            nvgBeginPath(vg);
            nvgCircle(vg, 10.0f, 10.0f, 1.0f);
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
            nvgEllipse(vg, 140.0f, 20.f, 25.0f, 15.0f);
            nvgFillColor(vg, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
            nvgFill(vg);
            nvgStrokeColor(vg, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
            nvgStrokeWidth(vg, 2.0f);
            nvgStroke(vg);

            // Render Sprite default
            NVGpaint paint = nvgImagePattern(vg, 150.0f, 10.0f + 128.0f, 128.0f, -128.0f, 0.0f, nvg_sprite, 1.0f);
            nvgBeginPath(vg);
            nvgRect(vg, 180.0f, 10.0f, 128.0f, 128.0f);
            nvgFillPaint(vg, paint);
            nvgFill(vg);

            // Render Font
            nvgFontFaceId(vg, font);
            nvgFontSize(vg, 24.0f);
            nvgFillColor(vg, nvgRGBAf(1.0f, 1.0f, 1.0f, 1.0f));
            nvgText(vg, 20.0f, 100.0f, "Hello World", nullptr);

            nvgEndFrame(vg);

            // Render Sprite effects
            s_sprite.Use();
            float aspect = (float)width / (float)height;
            glUniform2f(glGetUniformLocation(s_sprite.Get(), "u_pos"), 0.0125f, 0.75f);
            glUniform2f(glGetUniformLocation(s_sprite.Get(), "u_scale"), 0.21f, 0.21f);
            glUniform1f(glGetUniformLocation(s_sprite.Get(), "u_rotation"), glm::radians(0.0f));
            glUniform2f(glGetUniformLocation(s_sprite.Get(), "u_tex_scale"), 1.0f, 1.0f);
            glUniform4f(glGetUniformLocation(s_sprite.Get(), "u_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform1f(glGetUniformLocation(s_sprite.Get(), "u_time"), (float)glfwGetTime());
            glUniform1f(glGetUniformLocation(s_sprite.Get(), "u_intensity"), 0.5f);
            glUniform1f(glGetUniformLocation(s_sprite.Get(), "u_aspect"), aspect);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, img_sprite.Get());
            m_sprite.Draw();

            // Render Metaball
            s_metalball.Use();
            const glm::vec2 m_center[]{{0.3f, 0.75f}, {0.4f, 0.80f}, {0.45f, 0.70f}};
            const float m_radius[]{0.03f, 0.04f, 0.03f};
            const GLsizei count = sizeof(m_center) / sizeof(m_center[0]);
            glUniform2fv(glGetUniformLocation(s_metalball.Get(), "u_center"), count, &m_center[0].x);
            glUniform1fv(glGetUniformLocation(s_metalball.Get(), "u_radius"), count, m_radius);
            glUniform1i(glGetUniformLocation(s_metalball.Get(), "u_count"), count);
            glUniform4f(glGetUniformLocation(s_metalball.Get(), "u_fill_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform4f(glGetUniformLocation(s_metalball.Get(), "u_outline_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform1f(glGetUniformLocation(s_metalball.Get(), "u_threshold"), 0.6f);
            glUniform1f(glGetUniformLocation(s_metalball.Get(), "u_thickness"), 0.2f);
            glUniform1i(glGetUniformLocation(s_metalball.Get(), "u_outline"), true);
            m_metalball.Draw();

            SwapBuffers();
            PollEvents();
        }

        // De
        s_metalball.Delete();
        m_metalball.Delete();
        s_sprite.Delete();
        m_sprite.Delete();
        img_sprite.Delete();
    }
};

// Função principal do programa
int main()
{
    MyProgram myProgram;      // Cria uma instância do programa que gerencia a janela e o contexto OpenGL
    myProgram.RunEventLoop(); // Executa o loop de eventos da janela, que inclui a lógica de renderização
    return 0;                 // Retorna 0 para indicar que o programa terminou com sucesso
}