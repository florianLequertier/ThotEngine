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
//#include <fmod/fmod.h>

#include "thotEngine/GLEntity.hpp"
#include "thotEngine/Material.hpp"
#include "thotEngine/Image.hpp"
#include "thotEngine/GLProgram.hpp"
#include "thotEngine/Material.hpp"
#include "thotEngine/Mesh.hpp"

namespace te{

class ResourceManager
{

private:
    //meshes
    std::map<std::string, std::shared_ptr<Mesh> > m_meshes; // name <-> mesh
    std::map<std::string, std::string> m_meshKeys; // name <-> path
    std::map<std::string, int> m_meshCount; // name <-> count

    //images
    std::map<std::string, std::shared_ptr<Image> > m_images; //name <-> image
    std::map<std::string, std::string> m_imageKeys; // name <-> path
    std::map<std::string, int> m_imageCount; // name <-> count

    //glProgram
    std::map<std::string, std::shared_ptr<GLProgram> > m_programs; //name <-> program
    std::map<std::string, std::vector<std::string>> m_programKeys; // name <-> path
    std::map<std::string, int> m_programCount; // name <-> count

    //application path
    std::string m_applicationPath;

public:
    ResourceManager();
    ResourceManager(std::string applicationPath);
    ~ResourceManager();
    void init(std::string applicationPath);

    void pushMeshToGPU(std::string name);
    void popMeshFromGPU(std::string name);
    void pushImageToGPU(std::string name);
    void popImageFromGPU(std::string name);

    std::shared_ptr<Mesh> loadMesh(std::string name, std::string path, bool relative = true);
    std::shared_ptr<Mesh> loadMesh(std::string name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, bool relative = true);

    std::shared_ptr<Image> loadImage(std::string name, std::string path, bool relative = true);

    std::shared_ptr<GLProgram> loadProgram(std::string name, std::string vsPath, std::string fsPath, bool relative = true);

    std::string getApplicationPath() const;

    std::shared_ptr<Mesh> getMesh(std::string name) const;
    std::shared_ptr<Image> getImage(std::string name) const;
    std::shared_ptr<GLProgram> getProgram(std::string name) const;

};

}


#endif // RESOURCEMANAGER_HPP
