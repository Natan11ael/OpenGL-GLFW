// @file GLMesh.cpp
// @brief Implementação da classe GLMesh para gerenciar recursos de mesh e textura no OpenGL
// @details Esta classe encapsula o processo de carregamento, configuração e limpeza dos recursos OpenGL associados a um mesh, incluindo Vertex Array Objects, Vertex Buffer Objects e Element Buffer Objects.
//
// @author Natanael de Sousa
// @date 2026-05-23
// ------------------------------------------------------------------------------------------------------
//
#pragma once

// C++ standard library
#include <vector>

// Estrutura para descrever os atributos de vértice usados no mesh, incluindo o índice do atributo, o número de componentes e o tipo de dados
struct GLVertexAttribute
{
    unsigned int index; // Índice do atributo de vértice, usado para associar os dados de vértice aos atributos do shader
    int size;           // Número de componentes do atributo (por exemplo, 3 para posição XYZ, 2 para coordenadas de textura UV)
    unsigned int type;  // Tipo de dados do atributo (por exemplo, GL_FLOAT para dados de ponto flutuante, GL_INT para dados inteiros)
};

// Gerencia os recursos de mesh e textura para o programa, incluindo carregamento, configuração e limpeza dos recursos OpenGL
class GLMesh
{
private:
    unsigned int VAO, VBO, EBO; // IDs do Vertex Array Object, Vertex Buffer Object e Element Buffer Object usados para armazenar os dados de vértice e índice do mesh
    int vertex_count;           // Número de vértices no mesh, usado para desenhar o mesh corretamente

public:
    ~GLMesh();                                                                                                                                  // Destrutor para limpar os recursos do VAO, VBO e EBO
    bool Load(float *vertices, size_t vertex_size, unsigned int *indices, size_t index_size, const std::vector<GLVertexAttribute> &attributes); // Função para carregar os dados de vértice e índice para o mesh e configurar os buffers OpenGL
    void Draw();                                                                                                                                // Função para desenhar o mesh usando os buffers configurados
    void Delete();                                                                                                                              // Função para limpar os recursos do VAO, VBO e EBO associados ao mesh
};