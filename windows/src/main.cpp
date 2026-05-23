// @file main.cpp
// @brief Exemplo de programa que carrega uma textura usando stb_image e a exibe em um quadrado usando OpenGL e GLFW
// @details Este programa demonstra como usar a biblioteca stb_image para carregar uma imagem, criar uma textura OpenGL e renderizá-la em um quadrado que cobre toda a janela. Ele também inclui shaders simples para passar as coordenadas de textura e amostrar a textura no fragment shader.
//
// @author Natanael de Sousa
// @date 2026-05-22
// @version 0.0.0 Pre-alpha (Desenvolvimento Ativo)
//
// @copyright Copyright (c) 2026 - Todos os direitos reservados.
// ------------------------------------------------------------------------------------------------------
//

// stb_image - v2.27 - public domain image loader - http://nothings.org/stb_image.h
// no warranty implied; use at your own risk
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

// OpenGL e GLFW
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//

// C++ standard library
#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <iterator>
#include <string>

// Define STBI_rgb_alpha para garantir que as imagens sejam carregadas com 4 canais (RGBA)
#ifndef STBI_rgb_alpha
#define STBI_rgb_alpha 4
#endif

// Cria um quadrado cobrindo toda a tela
unsigned int createQuad()
{
    // Vértices para um quadrado cobrindo toda a tela, com posições e coordenadas de textura
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f};

    // Índices para desenhar o quadrado usando um elemento buffer (EBO)
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    // Gerencia os buffers de vértice e array de vértice para o quadrado
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the vertex array object and buffers, and upload the vertex data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind the element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configura os atributos de vértice para as posições
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Configura os atributos de vértice para as coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO

    return VAO;
}

// Gerencia os shaders do programa, incluindo compilação, linkagem e uso do programa de shader OpenGL
class GLShader
{
private:
    unsigned int m_program; // ID do programa de shader OpenGL

public:
    ~GLShader() // Destrutor para limpar os recursos do programa de shader OpenGL
    {
        Delete();
    }

    // Função para compilar um shader a partir do código-fonte do shader
    unsigned int GetCompileShader(unsigned int shader_type, const char *shader_source)
    {
        unsigned int shader_id = glCreateShader(shader_type); // Cria um shader do tipo especificado (GL_VERTEX_SHADER ou GL_FRAGMENT_SHADER)

        glShaderSource(shader_id, 1, &shader_source, NULL);
        glCompileShader(shader_id);

        GLint success;
        GLchar infoLog[1024];
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader_id, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: GL_SHADER" << std::endl
                      << infoLog << std::endl;
        }

        return shader_id;
    }

    // Função para carregar, compilar e linkar os shaders de vértice e fragmento a partir dos arquivos de shader
    bool Load(const char *vertex_path, const char *fragment_path)
    {
        std::ifstream is_vs(vertex_path), is_fs(fragment_path);

        std::stringstream buffer_vs, buffer_fs;
        buffer_vs << is_vs.rdbuf();
        buffer_fs << is_fs.rdbuf();

        const std::string f_vs = buffer_vs.str();
        const std::string f_fs = buffer_fs.str();

        m_program = glCreateProgram();

        unsigned int vs = GetCompileShader(GL_VERTEX_SHADER, f_vs.c_str());
        unsigned int fs = GetCompileShader(GL_FRAGMENT_SHADER, f_fs.c_str());

        glAttachShader(m_program, vs);
        glAttachShader(m_program, fs);

        glLinkProgram(m_program);
        glValidateProgram(m_program);

        GLint success;
        GLchar infoLog[1024];
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_program, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: GL_PROGRAM" << std::endl
                      << infoLog << "\n";
        }

        glDeleteShader(vs);
        glDeleteShader(fs);

        return true;
    }

    // Função para usar o programa de shader OpenGL para renderização
    void Use()
    {
        glUseProgram(m_program);
    }

    // Função para deletar o programa de shader OpenGL e liberar os recursos associados
    void Delete()
    {
        glDeleteProgram(m_program);
    }
};

struct GLVertexAttribute
{
    unsigned int index;
    int size;
    unsigned int type;
};

// Gerencia os recursos de mesh e textura para o programa, incluindo carregamento, configuração e limpeza dos recursos OpenGL
class GLMesh
{
private:
    unsigned int VAO, VBO, EBO;
    int vertex_count;

public:
    ~GLMesh() // Destrutor para limpar os recursos do VAO, VBO e EBO
    {
        Delete();
    }

    // Função para carregar os dados de vértice e índice para o mesh e configurar os buffers OpenGL
    bool Load(float *vertices, size_t vertex_size, unsigned int *indices, size_t index_size, const std::vector<GLVertexAttribute> &attributes)
    {
        Delete(); // Limpa os recursos anteriores, se houver

        vertex_count = (int)(index_size / sizeof(unsigned int));

        // Gerencia os buffers de vértice e array de vértice para o mesh
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO); // Bind do VAO para configurar os buffers e atributos de vértice

        // Bind do VBO e upload dos dados de vértice
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertex_size, vertices, GL_STATIC_DRAW);

        // Bind do EBO e upload dos dados de índice
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, indices, GL_STATIC_DRAW);

        // Calcula o stride total dos atributos de vértice para configurar os ponteiros de atributo corretamente
        int total_components = 0;
        for (const auto &attr : attributes)
        {
            total_components += attr.size;
        }
        GLsizei stride = total_components * sizeof(float);

        // Configura os atributos de vértice com base na descrição fornecida e habilita os arrays de vértice correspondentes
        size_t offset = 0;
        for (const auto &attr : attributes)
        {
            glVertexAttribPointer(
                attr.index,
                attr.size,
                attr.type,
                GL_FALSE,
                stride,
                (void *)offset);
            glEnableVertexAttribArray(attr.index);

            // Avança o offset na memória baseando-se no tamanho do atributo atual
            if (attr.type == GL_FLOAT)
                offset += attr.size * sizeof(float);
            else if (attr.type == GL_INT)
                offset += attr.size * sizeof(int);
            else if (attr.type == GL_UNSIGNED_BYTE)
                offset += attr.size * sizeof(unsigned char);
        }

        glBindVertexArray(0); // Unbind VAO
        return true;
    }

    void Draw()
    {
        if (vertex_count == 0)
            return;
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Delete()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};

// Gerencia o carregamento de recursos para o programa (em desenvolvimento, pode ser expandida para incluir mais tipos de recursos no futuro)
class GLImage
{
private:
    unsigned int asset_id; // ID do recurso de textura OpenGL

public:
    // Função para carregar recursos de textura usando stb_image e criar texturas OpenGL
    bool Load(const char *path)
    {
        stbi_set_flip_vertically_on_load(true);

        int width, height, channels;
        unsigned char *data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
        if (!data)
        {
            std::cerr << "Falha ao carregar imagem: " << path << "\n";
            return false;
        }

        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
        asset_id = tex;
        return true;
    }

    unsigned int Get() const
    {
        return asset_id;
    }

    void Delete()
    {
        glDeleteTextures(1, &asset_id);
    }
};

// Gerencia a renderização (em desenvolvimento, pode ser expandida para incluir mais funcionalidades de renderização no futuro)
namespace Drawing2D
{
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
};

// Callback para redimensionamento da janela, que atualiza o viewport do OpenGL para corresponder ao novo tamanho da janela
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Atualiza o OpenGL com o novo tamanho da janela
    glViewport(0, 0, width, height);
}

// Gerencia a janela e o contexto OpenGL usando GLFW
class WindowManager
{
private:
    GLFWwindow *window; // Ponteiro para a janela GLFW

public:
    // Construtor que inicializa GLFW e cria uma janela
    WindowManager(int width, int height, const char *title, GLFWmonitor *monitor = NULL, GLFWwindow *share = NULL)
    {
        // Inicializa GLFW
        if (!glfwInit())
        {
            std::cerr << "Falha ao inicializar GLFW\n";
            throw std::runtime_error("Falha ao inicializar GLFW");
        }

        // Configura as dicas para a criação do contexto OpenGL (versão 3.3 core profile)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // Define a versão principal do OpenGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // Define a versão menor do OpenGL
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Define o perfil do OpenGL para core

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Necessário para rodar em macOS
#endif

        // Cria uma janela GLFW
        window = glfwCreateWindow(width, height, title, monitor, share);
        if (!window)
        {
            std::cerr << "Falha ao criar janela GLFW\n";
            glfwTerminate();
            throw std::runtime_error("Falha ao criar janela GLFW");
        }

        // Torna o contexto OpenGL da janela atual
        glfwMakeContextCurrent(window);

        // Inicializa GLAD para carregar as funções do OpenGL
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Falha ao inicializar GLAD\n";
            glfwDestroyWindow(window);
            glfwTerminate();
            throw std::runtime_error("Falha ao inicializar GLAD");
        }

        // Configura o viewport e o callback para redimensionamento da janela
        glViewport(0, 0, width, height);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Configura o callback para redimensionamento da janela
        std::cout << "Inicializando Execucao..." << std::endl;
    }

    // Destrutor que limpa os recursos da janela e termina GLFW
    ~WindowManager()
    {
        if (window) // Verifica se a janela foi criada antes de tentar destruí-la
            glfwDestroyWindow(window);
        glfwTerminate(); // Garante que GLFW seja terminado mesmo que a janela não tenha sido criada com sucesso
    }

    virtual void inputsHandler()
    {
        // Função para lidar com entradas do usuário (em desenvolvimento, pode ser expandida para incluir mais tipos de entrada no futuro)

        // teste de entrada: fecha a janela quando a tecla ESC for pressionada
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            std::cout << "Finalizando Execucao..." << std::endl;
            glfwSetWindowShouldClose(window, true);
        }
    }

    // Função para verificar se a janela deve ser fechada
    bool shouldClose() const
    {
        return glfwWindowShouldClose(window);
    }

    // Função para trocar os buffers da janela
    void swapBuffers() const
    {
        glfwSwapBuffers(window);
    }

    // Função para processar eventos da janela
    void pollEvents() const
    {
        glfwPollEvents();
    }

    // Função para executar o loop de eventos da janela padrão (pode ser sobrescrita para adicionar lógica de renderização)
    virtual void runEventLoop()
    {
        // Loop principal de eventos da janela
        while (!shouldClose())
        {
            inputsHandler(); // Lida com entradas do usuário
            swapBuffers();
            pollEvents();
        }
    }

    // Função para obter o ponteiro da janela (se necessário para outras operações)
    GLFWwindow *getWindow() const
    {
        return window;
    }

    // Desabilita a cópia do WindowManager para evitar problemas de gerenciamento de recursos
    WindowManager(const WindowManager &) = delete;
    WindowManager &operator=(const WindowManager &) = delete;
};

// Classe que herda de WindowManager para implementar a lógica de renderização específica do programa
class MyProgram : public WindowManager
{
public:
    MyProgram() : WindowManager(800, 600, "OpenGL Texture Example") {}

    // Função para executar o loop de eventos da janela
    void runEventLoop()
    {
        // Configura o dataset para o loop de eventos

        // Carrega os shaders usando a classe GLShader.
        GLShader shader;                                                                          // Cria uma instância do shader para gerenciar o programa de shader OpenGL
        if (!shader.Load("../src/assets/GLSL/point.vs.glsl", "../src/assets/GLSL/point.fs.glsl")) // Carrega os shaders de vértice e fragmento a partir dos arquivos
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
        GLImage texture;                                // Cria uma instância do GLImage para gerenciar a textura do programa
        if (!texture.Load("../src/assets/texture.png")) // Carrega a textura a partir do arquivo usando stb_image e cria uma textura OpenGL
        {
            std::cerr << "Falha ao carregar imagem. Verifique o caminho.\n";
            return;
        }

        // Loop principal de eventos da janela
        while (!shouldClose())
        {
            // Lógica de entrada e renderização
            inputsHandler(); // Lida com entradas do usuário

            // Lógica de renderização
            glClear(GL_COLOR_BUFFER_BIT); // Limpa a tela para desenhar o próximo frame

            Drawing2D::Sprite(&mesh, &shader, texture.Get(), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)); // Desenha a textura usando o shader e o mesh configurados

            swapBuffers();
            pollEvents();
        }

        shader.Delete();  // Deleta o programa de shader para liberar os recursos associados
        mesh.Delete();    // Deleta o mesh para liberar os recursos associados
        texture.Delete(); // Deleta a textura para liberar os recursos associados
    }
};

// Função principal do programa
int main()
{
    MyProgram program;      // Cria uma instância do programa que gerencia a janela e o contexto OpenGL
    program.runEventLoop(); // Executa o loop de eventos da janela, que inclui a lógica de renderização
    return 0;               // Retorna 0 para indicar que o programa terminou com sucesso
}