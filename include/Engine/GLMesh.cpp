// @file GLMesh.cpp
// @brief Implementação da classe GLMesh para gerenciar recursos de mesh e textura no OpenGL
// @details Esta classe encapsula o processo de carregamento, configuração e limpeza dos recursos OpenGL associados a um mesh, incluindo Vertex Array Objects, Vertex Buffer Objects e Element Buffer Objects.
//
// @author Natanael de Sousa
// @date 2026-05-23
// ------------------------------------------------------------------------------------------------------
//

// OpenGL e GLFW headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Engine headers
#include "GLMesh.hpp"

GLMesh::~GLMesh() // Destrutor para limpar os recursos do VAO, VBO e EBO
{
    Delete();
}

// Função para carregar os dados de vértice e índice para o mesh e configurar os buffers OpenGL
bool GLMesh::Load(float *vertices, size_t vertex_size, unsigned int *indices, size_t index_size, const std::vector<GLVertexAttribute> &attributes)
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

void GLMesh::Draw()
{
    if (vertex_count == 0)
        return;
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void GLMesh::Delete()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
