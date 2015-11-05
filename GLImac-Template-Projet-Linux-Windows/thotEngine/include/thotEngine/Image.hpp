#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <iostream>
#include <cstdlib>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <gl/glew.h>

namespace te{

class Image
{

private:
    std::string m_name;
    unsigned int m_width = 0u;
    unsigned int m_height = 0u;
    std::unique_ptr<glm::vec4[]> m_Pixels;
    GLuint m_glId;

public:
    Image(std::string name, unsigned int width, unsigned int height);
    void pushToGPU();
    void popFromGPU();

    inline std::string getName() const {
        return m_name;
    }

    unsigned int getWidth() const {
        return m_width;
    }

    unsigned int getHeight() const {
        return m_height;
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
