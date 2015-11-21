
#include "thotEngine/cubemap.hpp"

namespace te{

CubeMap::CubeMap(std::string name) : m_name(name), m_glId(-1), m_width(1), m_height(1)
{

}

CubeMap::CubeMap(std::string name, unsigned int width, unsigned int height): m_name(name), m_glId(-1), m_width(width), m_height(height)
{

}

void CubeMap::pushToGPU()
{
    glGenTextures(1, &m_glId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_glId);

        for(int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_FLOAT, getPixels(i));
        }

        glEnable(GL_TEXTURE_CUBE_MAP);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::popFromGPU()
{
    glDeleteTextures(1, &m_glId);
}

GLuint CubeMap::getId() const
{
    return m_glId;
}

void CubeMap::addEmptyImage()
{
    m_Pixels.push_back( std::unique_ptr<glm::vec4[]>( new glm::vec4[m_width * m_height]) );
}

void CubeMap::setImgDim(int width,int height)
{
    m_width = width;
    m_height = height;
}

}
