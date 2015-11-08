#include "thotEngine/Image.hpp"

namespace te{

Image::Image(std::string name, unsigned int width, unsigned int height): m_name(name), m_glId(-1), m_width(width), m_height(height), m_Pixels(new glm::vec4[width * height])
{

}

void Image::pushToGPU()
{
    glGenTextures(1, &m_glId);
    glBindTexture(GL_TEXTURE_2D, m_glId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_FLOAT, getPixels());

        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // TODO add a global state for texture resolution
//        if(confData.getMipMapFilter())
//        {
            glEnable(GL_TEXTURE_2D);
            glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        }
//        else
//        {
//            if(confData.getMinFilter()==0)
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//            else if(confData.getMinFilter()==1)
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

//            if(confData.getMaxFilter()==0)
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//            if(confData.getMaxFilter()==1)
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Image::popFromGPU()
{
    glDeleteTextures(1, &m_glId);
}

GLuint Image::getId() const
{
    return m_glId;
}

}

