
#include "thotEngine/Material.hpp"

namespace te{

Material::Material(std::weak_ptr<ResourceManager> resourceManager, std::shared_ptr<GLProgram> program)
{
    m_resourceManager = resourceManager;
    m_program = program;
}

Material::Material(std::weak_ptr<ResourceManager> resourceManager, std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images)
{
    m_resourceManager = resourceManager;
    m_program = program;
    m_images = images;
}

Material::~Material()
{

}

void Material::pushToGPU() // !!!
{
    for(auto img : m_images)
    {
        m_resourceManager->pushImageToGPU(img->getName());
    }
}

void Material::popFromGPU()
{
    for(auto img : m_images)
    {
        m_resourceManager->popImageFromGPU(img->getName());
    }
}

}
