#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#include <iostream>
#include <cstdlib>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <gl/glew.h>

namespace te{

class CubeMap
{

private:
    std::string m_name;
    unsigned int m_width = 0u;
    unsigned int m_height = 0u;
    std::vector< std::unique_ptr<glm::vec4[]> > m_Pixels;
    GLuint m_glId;

public:
    CubeMap(std::string name);
    CubeMap(std::string name, unsigned int width, unsigned int height);
    void pushToGPU();
    void popFromGPU();

    GLuint getId() const;
    void addEmptyImage();
    void setImgDim(int width, int height);

    inline std::string getName() const {
        return m_name;
    }

    unsigned int getWidth() const {
        return m_width;
    }

    unsigned int getHeight() const {
        return m_height;
    }

    const glm::vec4* getPixels(int i ) const {
        return m_Pixels[i].get();
    }

    glm::vec4* getPixels(int i ) {
        return m_Pixels[i].get();
    }



};

}

#endif // CUBEMAP_HPP

