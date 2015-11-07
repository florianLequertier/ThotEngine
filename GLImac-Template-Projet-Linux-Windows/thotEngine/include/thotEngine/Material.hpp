#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <memory>
#include "GLProgram.hpp"
#include "Image.hpp"
#include "ResourceManager.hpp"

namespace te{

class Material
{
private:

    //std::weak_ptr<ResourceManager> m_resourceManager;

    std::weak_ptr<GLProgram> m_program;
    std::vector<std::weak_ptr<Image>> m_images;
    std::vector<GLuint> m_uniforms;

public:
    Material( std::shared_ptr<GLProgram> program );
    Material( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images );
    ~Material();

    void pushToGPU();
    void popFromGPU();

    GLuint getGlId() const;

    void initUniforms();
    virtual void setUniforms(const glm::mat4& modelMat, const glm::mat4& worldMat, const glm::mat4& viewMat);

};

}

#endif // MATERIAL_HPP
