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
                float v_triangle[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
                std::vector<GLVertexAttribute> att_triangle = {
                    {0, 2, GL_FLOAT}, // Atributo de posição (index 0, 2 componentes, tipo float)
                };

                // Carrega os dados de vértice e índice para o mesh e configura os buffers
                if (!m_triangle.Load(v_triangle, sizeof(v_triangle), nullptr, 0, att_triangle, GL_LINE_LOOP)) // GL_LINE_LOOP = linhas | GL_TRIANGLES = solido
                    LOGE("Falha ao carregar mesh.");

                //
                // Carrega os shaders usando a classe GLShader.
                if (!s_sprite.Load("Shaders/sprite.vs.glsl", "Shaders/sprite.fs.glsl")) // Carrega os shaders de vértice e fragmento a partir dos arquivos
                    LOGE("Falha ao carregar shaders.");

                // Cria um quadrado cobrindo toda a tela e obtém o ID do VAO para renderização
                float v_sprite[] = {
                    -1.0f, -1.0f, 0.0f, 0.0f,
                    1.0f, -1.0f, 1.0f, 0.0f,
                    1.0f, 1.0f, 1.0f, 1.0f,
                    -1.0f, 1.0f, 0.0f, 1.0f};
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
            const glm::vec2 p_point{0.0f, 0.0f};
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
            glUniform4f(glGetUniformLocation(s_triangle.Get(), "u_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            glLineWidth(2.0f);
            m_triangle.Draw();

            // Render Sprite
            // s_sprite.Use();                                 // Usa o programa de shader para renderização
            // glActiveTexture(GL_TEXTURE0);                   // Ativa a unidade de textura 0 para amostrar a textura no shader
            // glBindTexture(GL_TEXTURE_2D, img_sprite.Get()); // Liga a textura para que o shader possa amostrá-la
            // m_sprite.Draw();                                // Desenha o mesh usando os dados de vértice e índice carregados

            SwapBuffers();
            PollEvents();
        }

        // D
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