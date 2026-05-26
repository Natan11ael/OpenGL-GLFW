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

// Engine headers
#include "GLWindow.hpp"
#include "Engine/GLShader.hpp"
#include "Engine/GLMesh.hpp"
#include "Engine/GLImage.hpp"
#include "Engine/GLLog.hpp"

// C++ standard library
#include <iostream>

void Rect(GLMesh layer, glm::vec2 position, float width, float height)
{
    // Função para desenhar um retângulo (em desenvolvimento, pode ser implementada usando OpenGL para desenhar formas básicas)
}

void Ellipse(GLMesh layer, glm::vec2 center, glm::vec2 radius)
{
    // Função para desenhar uma elipse (em desenvolvimento, pode ser implementada usando OpenGL para desenhar formas básicas)
}

void Metalball(GLMesh layer, glm::vec2 position, float radius)
{
    // Função para desenhar uma metalball (em desenvolvimento, pode ser implementada usando OpenGL para desenhar formas básicas)
}

void Sprite(GLMesh *layer, GLShader *shader, unsigned int image, glm::vec2 position, glm::vec2 scale) {}

// Classe que herda de WindowManager para implementar a lógica de renderização específica do programa
class MyProgram : public GLWindow
{
public:
    MyProgram() : GLWindow(800, 600, "OpenGL Example") {}

    // Função para executar o loop de eventos da janela
    void RunEventLoop()
    {
        // Configura o dataset para o loop de eventos
        GLShader s_point, s_line, s_triangle, s_rect, s_ellipse, s_metalball, s_sprite;
        GLMesh m_point, m_line, m_triangle, m_rect, m_ellipse, m_metalball, m_sprite;
        GLImage img_sprite;
        bool loaded = false;

        // Loop principal de eventos da janela
        while (!ShouldClose())
        {
            // Lógica de entrada e renderização
            InputsHandler(); // Lida com entradas do usuário

            //
            if (!loaded)
            {
                //
                // Carrega os shaders usando a classe GLShader.
                if (!s_point.Load("Shaders/point.vs.glsl", "Shaders/point.fs.glsl")) // Carrega os shaders de vértice e fragmento a partir dos arquivos
                    LOGE("Falha ao carregar shaders.");

                // Cria um quadrado cobrindo toda a tela e obtém o ID do VAO para renderização
                float v_point[] = {0.0f, 0.0f};
                std::vector<GLVertexAttribute> att_point = {
                    {0, 2, GL_FLOAT}, // Atributo de posição (index 0, 2 componentes, tipo float)
                };

                // Carrega os dados de vértice e índice para o mesh e configura os buffers
                if (!m_point.Load(v_point, sizeof(v_point), nullptr, 0, att_point, GL_POINTS))
                    LOGE("Falha ao carregar mesh.");

                //
                // Carrega os shaders usando a classe GLShader.
                if (!s_line.Load("Shaders/line.vs.glsl", "Shaders/line.fs.glsl")) // Carrega os shaders de vértice e fragmento a partir dos arquivos
                    LOGE("Falha ao carregar shaders.");

                // Cria um quadrado cobrindo toda a tela e obtém o ID do VAO para renderização
                float v_line[] = {0.0f, 0.0f, 1.0f, 1.0f};
                std::vector<GLVertexAttribute> att_line = {
                    {0, 2, GL_FLOAT}, // Atributo de posição (index 0, 2 componentes, tipo float)
                };

                // Carrega os dados de vértice e índice para o mesh e configura os buffers
                if (!m_line.Load(v_line, sizeof(v_line), nullptr, 0, att_line, GL_LINES))
                    LOGE("Falha ao carregar mesh.");

                //
                // Carrega os shaders usando a classe GLShader.
                if (!s_triangle.Load("Shaders/triangle.vs.glsl", "Shaders/triangle.fs.glsl")) // Carrega os shaders de vértice e fragmento a partir dos arquivos
                    LOGE("Falha ao carregar shaders.");

                // Cria um quadrado cobrindo toda a tela e obtém o ID do VAO para renderização
                float v_triangle[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
                std::vector<GLVertexAttribute> att_triangle = {
                    {0, 2, GL_FLOAT}, // Atributo de posição (index 0, 2 componentes, tipo float)
                };

                // Carrega os dados de vértice e índice para o mesh e configura os buffers
                if (!m_triangle.Load(v_triangle, sizeof(v_triangle), nullptr, 0, att_triangle, GL_TRIANGLES)) // GL_LINE_LOOP = linhas | GL_TRIANGLES = solido
                    LOGE("Falha ao carregar mesh.");

                //
                // Carrega os shaders usando a classe GLShader.
                if (!s_rect.Load("Shaders/rect.vs.glsl", "Shaders/rect.fs.glsl")) // Carrega os shaders de vértice e fragmento a partir dos arquivos
                    LOGE("Falha ao carregar shaders.");

                // Cria um quadrado cobrindo toda a tela e obtém o ID do VAO para renderização
                float v_rect[] = {-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};
                unsigned int id_rect[] = {0, 1, 2, 2, 3, 0};
                std::vector<GLVertexAttribute> att_rect = {
                    {0, 2, GL_FLOAT}, // Atributo de posição (index 0, 2 componentes, tipo float)
                };

                // Carrega os dados de vértice e índice para o mesh e configura os buffers
                if (!m_rect.Load(v_rect, sizeof(v_rect), id_rect, sizeof(id_rect), att_rect, GL_TRIANGLES))
                    LOGE("Falha ao carregar mesh.");

                //
                // Carrega os shaders usando a classe GLShader.
                if (!s_ellipse.Load("Shaders/ellipse.vs.glsl", "Shaders/ellipse.fs.glsl")) // Carrega os shaders de vértice e fragmento a partir dos arquivos
                    LOGE("Falha ao carregar shaders.");

                // Cria um quadrado cobrindo toda a tela e obtém o ID do VAO para renderização
                float v_ellipse[] = {-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};
                unsigned int id_ellipse[] = {0, 1, 2, 2, 3, 0};
                std::vector<GLVertexAttribute> att_ellipse = {
                    {0, 2, GL_FLOAT}, // Atributo de posição (index 0, 2 componentes, tipo float)
                };

                // Carrega os dados de vértice e índice para o mesh e configura os buffers
                if (!m_ellipse.Load(v_ellipse, sizeof(v_ellipse), id_ellipse, sizeof(id_ellipse), att_ellipse, GL_TRIANGLES))
                    LOGE("Falha ao carregar mesh.");

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

                //
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

                loaded = true;
            }

            // Lógica de renderização
            glClear(GL_COLOR_BUFFER_BIT); // Limpa a tela para desenhar o próximo frame

            // Render Point
            s_point.Use();
            const glm::vec2 p_point{0.0f, -0.1f};
            glUniform2fv(glGetUniformLocation(s_point.Get(), "u_pos"), 1, &p_point.x);
            glUniform4f(glGetUniformLocation(s_point.Get(), "u_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glEnable(GL_PROGRAM_POINT_SIZE);
            m_point.Draw();

            // Render Line
            s_line.Use();
            const glm::vec2 l_points[2]{{-0.4f, 0.4f}, {0.4f, 0.4f}};
            glUniform2fv(glGetUniformLocation(s_line.Get(), "u_point"), 2, &l_points[0].x);
            glUniform4f(glGetUniformLocation(s_line.Get(), "u_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            m_line.Draw();

            // Render Triangle
            s_triangle.Use();
            const glm::vec2 t_points[3]{{-0.4f, -0.4f}, {0.4f, -0.4f}, {0.0f, 0.4f}};
            glUniform2fv(glGetUniformLocation(s_triangle.Get(), "u_point"), 3, &t_points[0].x);
            glUniform4f(glGetUniformLocation(s_triangle.Get(), "u_fill_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform4f(glGetUniformLocation(s_triangle.Get(), "u_outline_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform1f(glGetUniformLocation(s_triangle.Get(), "u_thickness"), 0.02f);
            glUniform1i(glGetUniformLocation(s_triangle.Get(), "u_outline"), true);
            m_triangle.Draw();

            // Render Rect
            s_rect.Use();
            const glm::vec2 r_points[4]{{-0.3f, -0.3f}, {0.3f, -0.3f}, {0.3f, 0.3f}, {-0.3f, 0.3f}};
            glUniform2fv(glGetUniformLocation(s_rect.Get(), "u_point"), 4, &r_points[0].x);
            glUniform4f(glGetUniformLocation(s_rect.Get(), "u_fill_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform4f(glGetUniformLocation(s_rect.Get(), "u_outline_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform1f(glGetUniformLocation(s_rect.Get(), "u_thickness"), 0.02f);
            glUniform1i(glGetUniformLocation(s_rect.Get(), "u_outline"), true);
            m_rect.Draw();

            // Render Ellipse
            s_ellipse.Use();
            const glm::vec2 e_center{0.0f, 0.0f}, e_radius{0.4f, 0.2f};
            glUniform2fv(glGetUniformLocation(s_ellipse.Get(), "u_center"), 1, &e_center.x);
            glUniform2fv(glGetUniformLocation(s_ellipse.Get(), "u_radius"), 1, &e_radius.x);
            glUniform4f(glGetUniformLocation(s_ellipse.Get(), "u_fill_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform4f(glGetUniformLocation(s_ellipse.Get(), "u_outline_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glUniform1f(glGetUniformLocation(s_ellipse.Get(), "u_thickness"), 0.02f);
            glUniform1i(glGetUniformLocation(s_ellipse.Get(), "u_outline"), true);
            m_ellipse.Draw();

            // Render Ellipse
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

            // Render Sprite
            s_sprite.Use();
            const glm::vec2 s_point{-0.75f, -0.0f};
            glUniform2fv(glGetUniformLocation(s_sprite.Get(), "u_pos"), 1, &s_point.x);
            glUniform2f(glGetUniformLocation(s_sprite.Get(), "u_scale"), 0.25f, 0.25f);
            glUniform1f(glGetUniformLocation(s_sprite.Get(), "u_rotation"), glm::radians(0.0f));
            glUniform2f(glGetUniformLocation(s_sprite.Get(), "u_tex_scale"), 1.0f, 1.0f);
            glUniform4f(glGetUniformLocation(s_sprite.Get(), "u_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, img_sprite.Get());
            m_sprite.Draw();

            SwapBuffers();
            PollEvents();
        }

        // De
        s_point.Delete();
        s_line.Delete();
        s_triangle.Delete();
        s_rect.Delete();
        s_ellipse.Delete();
        s_metalball.Delete();
        s_sprite.Delete();
        m_point.Delete();
        m_line.Delete();
        m_triangle.Delete();
        m_rect.Delete();
        m_ellipse.Delete();
        m_metalball.Delete();
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