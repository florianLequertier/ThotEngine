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

    std::weak_ptr<ResourceManager> m_resourceManager;

    std::shared_ptr<GLProgram> m_program;
    std::shared_ptr<Image> m_images;

public:
    Material(std::weak_ptr<ResourceManager> resourceManager, std::shared_ptr<GLProgram> program);
    Material(std::weak_ptr<ResourceManager> resourceManager, std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images );
    ~Material();

    void pushToGPU();
    void popFromGPU();

};

}

#endif // MATERIAL_HPP
