#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <iostream>
#include <cstdlib>
#include <vector>
#include <glm/glm.hpp>

namespace te{

class Image
{

private:
    unsigned int m_nWidth = 0u;
    unsigned int m_nHeight = 0u;
    std::unique_ptr<glm::vec4[]> m_Pixels;

public:
    Image(unsigned int width, unsigned int height):
        m_nWidth(width), m_nHeight(height), m_Pixels(new glm::vec4[width * height]) {
    }

    unsigned int getWidth() const {
        return m_nWidth;
    }

    unsigned int getHeight() const {
        return m_nHeight;
    }

    const glm::vec4* getPixels() const {
        return m_Pixels.get();
    }

    glm::vec4* getPixels() {
        return m_Pixels.get();
    }

};

}

#endif // IMAGE_HPP
