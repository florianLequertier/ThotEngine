#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include <map>

#include <SDL\SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <fmod/fmod.h>

#include "thotEngine/GLEntity.hpp"
#include "thotEngine/Material.hpp"
#include "thotEngine/Image.hpp"
#include "thotEngine/GLProgram.hpp"
#include "thotEngine/Mesh.hpp"

namespace te{

class ResourceManager
{

private:
    //meshes
    std::map<std::string, std::shared_ptr<Mesh> > m_meshes;
    //std::map<std::string, int> m_meshesMapping;
    //images
    std::map<std::string, std::shared_ptr<Image> > m_images;
    //std::map<std::string, int> m_textureMapping;
    //glProgram
    std::map<std::string, std::shared_ptr<GLProgram> > m_programs;

    //application path
    std::string m_applicationPath;

public:
    ResourceManager(std::string applicationPath);
    ~ResourceManager();

    std::shared_ptr<Mesh> loadMesh(std::string name, std::string path);
    std::shared_ptr<Mesh> loadMesh(std::string name, const std::vector<gl::Vertex>& vertices, const std::vector<uint32_t>& indices);

    std::shared_ptr<Image> ResourceManager::loadImage(std::string name, std::string path);

    std::shared_ptr<GLProgram> loadProgram(std::string name, std::string vsRelativePath, std::string fsRelativePath);

    std::string getApplicationPath() const;

    std::shared_ptr<Material> createMaterial(std::string programName);
    std::shared_ptr<Material> createMaterial(std::string programName, std::vector<std::shared_ptr<gl::Image> > textures);

};

}


#endif // RESOURCEMANAGER_HPP
