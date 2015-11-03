
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm\ext.hpp>

#include "thotEngine/GLEntity.hpp"
#include "thotEngine/GLConfig.hpp"


namespace te{

GLEntity::GLEntity() : m_vbo(0), m_ibo(0), m_vao(0), m_indexNumber(0)
{
}


GLEntity::~GLEntity()
{
    clear();
}

GLEntity::GLEntity(const GLEntity& other)
{
    m_vbo = other.m_vbo;
    m_ibo = other.m_ibo;
    m_vao = other.m_vao;

    m_vertices = other.m_vertices;
    m_indices = other.m_indices;

    m_indexNumber = other.m_indexNumber;
    m_materialIndex = other.m_materialIndex;
}

GLEntity& GLEntity::operator=(const GLEntity& other)
{
    m_vbo = other.m_vbo;
    m_ibo = other.m_ibo;
    m_vao = other.m_vao;

    m_vertices = other.m_vertices;
    m_indices = other.m_indices;

    m_indexNumber = other.m_indexNumber;
    m_materialIndex = other.m_materialIndex;

    return *this;
}

void GLEntity::init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    clear();

    m_vertices = vertices;
    m_indices = indices;
    m_indexNumber = m_indices.size();
}

void GLEntity::pushToGPU()
{
    //creation du VBO :
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //creation de l'IBO :
    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32_t), &m_indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //Creation du VAO :
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

    glEnableVertexAttribArray(te::gl::GLINDEX_POSITION);
    glEnableVertexAttribArray(te::gl::GLINDEX_NORMAL);
    glEnableVertexAttribArray(te::gl::GLINDEX_TEXCOORDS);
    glEnableVertexAttribArray(te::gl::GLINDEX_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(te::gl::GLINDEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(te::Vertex), (const GLvoid*)offsetof(te::Vertex, position));
    glVertexAttribPointer(te::gl::GLINDEX_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(te::Vertex), (const GLvoid*)offsetof(te::Vertex, normal));
    glVertexAttribPointer(te::gl::GLINDEX_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(te::Vertex), (const GLvoid*)offsetof(te::Vertex, texCoords));
    glVertexAttribPointer(te::gl::GLINDEX_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(te::Vertex), (const GLvoid*)offsetof(te::Vertex, color));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void GLEntity::clear()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);
    glDeleteVertexArrays(1, &m_vao);

    m_vertices.clear();
    m_indices.clear();
}

void GLEntity::draw()
{
    glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_indexNumber, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

unsigned int GLEntity::getMaterialIndex() const
{
    return m_materialIndex;
}

void GLEntity::setMaterialIndex(unsigned int index)
{
    m_materialIndex = index;
}

}

