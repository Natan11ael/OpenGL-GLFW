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

// C++ standard library
#include <iostream>

void Point(GLMesh layer, glm::vec2 position)
{
    // Função para desenhar um ponto (em desenvolvimento, pode ser implementada usando OpenGL para desenhar formas básicas)
}

void Line(GLMesh layer, glm::vec2 start, glm::vec2 end)
{
    // Função para desenhar uma linha (em desenvolvimento, pode ser implementada usando OpenGL para desenhar formas básicas)
}

void Triangle(GLMesh layer, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
    // Função para desenhar um triângulo (em desenvolvimento, pode ser implementada usando OpenGL para desenhar formas básicas)
}

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

void Sprite(GLMesh *layer, GLShader *shader, unsigned int image, glm::vec2 position, glm::vec2 scale)
{
    shader->Use();                       // Usa o programa de shader para renderização
    glActiveTexture(GL_TEXTURE0);        // Ativa a unidade de textura 0 para amostrar a textura no shader
    glBindTexture(GL_TEXTURE_2D, image); // Liga a textura para que o shader possa amostrá-la
    layer->Draw();                       // Desenha o mesh usando os dados de vértice e índice carregados
}

// Classe que herda de WindowManager para implementar a lógica de renderização específica do programa
class MyProgram : public GLWindow
{
public:
    MyProgram() : GLWindow(800, 600, "OpenGL Example") {}

    // Função para executar o loop de eventos da janela
    void RunEventLoop()
    {
        // Configura o dataset para o loop de eventos

        // Carrega os shaders usando a classe GLShader.
        GLShader shader;                                                    // Cria uma instância do shader para gerenciar o programa de shader OpenGL
        if (!shader.Load("Shaders/point.vs.glsl", "Shaders/point.fs.glsl")) // Carrega os shaders de vértice e fragmento a partir dos arquivos
        {
            std::cerr << "Falha ao carregar shaders. Verifique os caminhos.\n";
            return;
        }

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

        GLMesh mesh;                                                                 // Cria uma instância do GLMesh para gerenciar os recursos de mesh do programa
        mesh.Load(vertices, sizeof(vertices), indices, sizeof(indices), attributes); // Carrega os dados de vértice e índice para o mesh e configura os buffers

        // Carrega a textura usando a classe GLImage.
        GLImage texture;                                    // Cria uma instância do GLImage para gerenciar a textura do programa
        if (!texture.Load("Textures/image.png")) // Carrega a textura a partir do arquivo usando stb_image e cria uma textura OpenGL
        {
            std::cerr << "Falha ao carregar imagem. Verifique o caminho.\n";
            return;
        }

        // Loop principal de eventos da janela
        while (!ShouldClose())
        {
            // Lógica de entrada e renderização
            InputsHandler(); // Lida com entradas do usuário

            // Lógica de renderização
            glClear(GL_COLOR_BUFFER_BIT); // Limpa a tela para desenhar o próximo frame

            Sprite(&mesh, &shader, texture.Get(), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)); // Desenha a textura usando o shader e o mesh configurados

            SwapBuffers();
            PollEvents();
        }

        shader.Delete();  // Deleta o programa de shader para liberar os recursos associados
        mesh.Delete();    // Deleta o mesh para liberar os recursos associados
        texture.Delete(); // Deleta a textura para liberar os recursos associados
    }
};

// Função principal do programa
int main()
{
    MyProgram myProgram;      // Cria uma instância do programa que gerencia a janela e o contexto OpenGL
    myProgram.RunEventLoop(); // Executa o loop de eventos da janela, que inclui a lógica de renderização
    return 0;                 // Retorna 0 para indicar que o programa terminou com sucesso
}