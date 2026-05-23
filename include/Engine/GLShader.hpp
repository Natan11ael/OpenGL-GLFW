// @file GLShader.hpp
// @brief Gerencia Shaders do programa, incluindo compilação, linkagem e uso do programa de shader OpenGL
// @details Esta classe encapsula o processo de carregamento, compilação e linkagem dos shaders de vértice e fragmento a partir de arquivos de shader. Ela também fornece métodos para usar o programa de shader durante a renderização e para limpar os recursos associados quando não forem mais necessários.
// 
// @author Natanael de Sousa
// @date 2026-05-23
// ------------------------------------------------------------------------------------------------------
//
#pragma once

// Gerencia os shaders do programa, incluindo compilação, linkagem e uso do programa de shader OpenGL
class GLShader
{
private:
    unsigned int m_program; // ID do programa de shader OpenGL

public:
    ~GLShader();                                                                        // Destrutor para limpar os recursos do programa de shader OpenGL
    unsigned int GetCompileShader(unsigned int shader_type, const char *shader_source); // Função para compilar um shader a partir do código-fonte do shader
    bool Load(const char *vertex_path, const char *fragment_path);                      // Função para carregar, compilar e linkar os shaders de vértice e fragmento a partir dos arquivos de shader
    void Use();                                                                         // Função para usar o programa de shader OpenGL para renderização
    void Delete();                                                                      // Função para deletar o programa de shader OpenGL e liberar os recursos associados
};