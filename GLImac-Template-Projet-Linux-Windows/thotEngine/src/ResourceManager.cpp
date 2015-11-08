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
#include "thotEngine/Mesh.hpp"
#include "thotEngine/Material.hpp"
#include "thotEngine/ResourceManager.hpp"

#include "thotEngine/Image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

namespace te{

//singleton (c++11 implementation)
ResourceManager& ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}

//privates
ResourceManager::ResourceManager() : m_applicationPath(""){}
ResourceManager::ResourceManager(const ResourceManager& other){}
ResourceManager::ResourceManager(const ResourceManager&& other){}
ResourceManager& ResourceManager::operator=(const ResourceManager& other){}

ResourceManager::~ResourceManager()
{
    m_meshes.clear();
    m_images.clear();
    m_programs.clear();
    //...
}

void ResourceManager::init(std::string applicationPath)
{
    m_applicationPath = applicationPath;
}

void ResourceManager::pushMeshToGPU(std::string name)
{
    //try to found mesh name among meshes which have already been loaded
    assert( m_meshKeys.find(name) != m_meshKeys.end() );

    //if mesh isn't present in scene (hasn't already be pushed to gpu)
    if(m_meshCount[name] == 0)
    {
        //push the mesh to the gpu
       m_meshes[name]->pushToGPU();
    }
    //update mesh use counter
    m_meshCount[name]++;
}

void ResourceManager::popMeshFromGPU(std::string name)
{
    //try to found mesh name among meshes which have already been loaded
    assert( m_meshKeys.find(name) != m_meshKeys.end() );

    //update mesh use counter
    m_meshCount[name]--;
    //if mesh has totaly disappeared from the sceen
    if(m_meshCount[name] == 0)
    {
       //pop the mesh to the gpu
       m_meshes[name]->popFromGPU();
    }
}

void ResourceManager::pushImageToGPU(std::string name)
{
    //try to found image name among meshes which have already been loaded
    assert( m_imageKeys.find(name) != m_imageKeys.end() );

    //if image isn't present in scene (hasn't already be pushed to gpu)
    if(m_imageCount[name] == 0)
    {
        //push the image to the gpu
       m_images[name]->pushToGPU();
    }
    //update image use counter
    m_imageCount[name]++;
}

void ResourceManager::popImageFromGPU(std::string name)
{
    //try to found image name among meshes which have already been loaded
    assert( m_imageKeys.find(name) != m_imageKeys.end() );

    //update image use counter
    m_imageCount[name]--;
    //if image has totaly disappeared from the sceen
    if(m_imageCount[name] == 0)
    {
       //pop the image to the gpu
       m_images[name]->popFromGPU();
    }
}

std::shared_ptr<Mesh> ResourceManager::loadMesh(std::string name, std::string path, bool relative)
{
    //complete path if the given path is relative
    if(relative)
    {
        path.insert(0, m_applicationPath);
    }

    if (m_meshes.find(name) != m_meshes.end())
    {
        //if a mesh with same name has already been created, return it
        return m_meshes[name];
    }
    else
    {
        //create a new mesh
        auto newMesh = std::shared_ptr<Mesh>(new Mesh(name));
        newMesh->load(path);

        //store it in the manager
        m_meshes[name] = newMesh;
        m_meshKeys[name] = path;
        m_meshCount[name] = 0;

        std::cout<< "loaded mesh " << path << "successfully !" << std::endl;
        return newMesh;
    }
}

std::shared_ptr<Mesh> ResourceManager::loadMesh(std::string name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    if (m_meshes.find(name) != m_meshes.end())
    {
        //if a mesh with same name has already been created, return it
        return m_meshes[name];
    }
    else
    {
        //create a new mesh
        auto newMesh = std::shared_ptr<Mesh>(new Mesh(name));
        newMesh->init(vertices, indices);

        //store it in the manager
        m_meshes[name] = newMesh;
        m_meshKeys[name] = ""; //no path, because it has been directly generated, not loaded from a file.
        m_meshCount[name] = 0;

        return newMesh;
    }
}

std::shared_ptr<Image> ResourceManager::loadImage(std::string name, std::string path, bool relative)
{
    //complete path if the given path is relative
    if(relative)
    {
        path.insert(0, m_applicationPath);
    }

    if (m_images.find(name) != m_images.end())
    {
        //if an image with same name has already been created, return it
        return m_images[name];
    }
    else
    {
        //create the new image
        int x, y, n;
        unsigned char *data = stbi_load(path.c_str(), &x, &y, &n, 4);
        if(!data) {
            std::cerr << "loading image " << path << " error: " << stbi_failure_reason() << std::endl;
            return std::shared_ptr<Image>();
        }
        std::shared_ptr<Image> pImage(new Image(name, x, y));
        unsigned int size = x * y;
        auto scale = 1.f / 255;
        auto ptr = pImage->getPixels();
        for(auto i = 0u; i < size; ++i) {
            auto offset = 4 * i;
            ptr->r = data[offset] * scale;
            ptr->g = data[offset + 1] * scale;
            ptr->b = data[offset + 2] * scale;
            ptr->a = data[offset + 3] * scale;
            ++ptr;
        }
        stbi_image_free(data);

        //store the new image in the manager
        m_images[name] = pImage;
        m_imageKeys[name] = path;
        m_imageCount[name] = 0;

        std::cout<< "loaded image " << path << "successfully !" << std::endl;
        return pImage;
    }
}

std::shared_ptr<GLProgram> ResourceManager::loadProgram(std::string name, std::string vsPath, std::string fsPath, bool relative)
{
    //complete path if the given path is relative
    if(relative)
    {
        vsPath.insert(0, m_applicationPath);
        fsPath.insert(0, m_applicationPath);
    }

    if (m_programs.find(name) != m_programs.end())
    {
        return m_programs[name];
    }
    else
    {
        //create the new program
        auto newProgram = std::make_shared<GLProgram>(name, vsPath, fsPath);
        newProgram->setProgramName(name);

        //store it in the manager
        m_programs[name] = newProgram;
        m_programKeys[name].push_back( vsPath);
        m_programKeys[name].push_back( fsPath);

        std::cout<< "loaded program " << vsPath <<", "<<fsPath<< " successfully !" << std::endl;
        return newProgram;
    }
}

std::string ResourceManager::getApplicationPath() const
{
    return m_applicationPath;
}

std::shared_ptr<Mesh> ResourceManager::getMesh(std::string name)
{
    return m_meshes[name];
}

std::shared_ptr<Image> ResourceManager::getImage(std::string name)
{
    return m_images[name];
}

std::shared_ptr<GLProgram> ResourceManager::getProgram(std::string name)
{
    return m_programs[name];
}

}

/*
std::unique_ptr<Image> loadImage(const FilePath& filepath)
{
    int x, y, n;
    unsigned char *data = stbi_load(filepath.c_str(), &x, &y, &n, 4);
    if(!data) {
        std::cerr << "loading image " << filepath << " error: " << stbi_failure_reason() << std::endl;
        return std::unique_ptr<Image>();
    }
    std::unique_ptr<Image> pImage(new Image(x, y));
    unsigned int size = x * y;
    auto scale = 1.f / 255;
    auto ptr = pImage->getPixels();
    for(auto i = 0u; i < size; ++i) {
        auto offset = 4 * i;
        ptr->r = data[offset] * scale;
        ptr->g = data[offset + 1] * scale;
        ptr->b = data[offset + 2] * scale;
        ptr->a = data[offset + 3] * scale;
        ++ptr;
    }
    stbi_image_free(data);
    return pImage;
}

std::unordered_map<FilePath, std::unique_ptr<Image>> ImageManager::m_ImageMap;

const Image* ResourceManager::loadImage(const FilePath& filepath)
{
    auto it = m_ImageMap.find(filepath);
    if(it != std::end(m_ImageMap)) {
        return (*it).second.get();
    }
    auto pImage = glimac::loadImage(filepath);
    if(!pImage) {
        return nullptr;
    }
    auto& img = m_ImageMap[filepath] = std::move(pImage);
    return img.get();
}


GLuint LoadImageFromFile(std::string path_string, Config& confData)
{
    std::unique_ptr<Image> ptr_texture = loadImage(path_string.c_str());

    if(ptr_texture ==NULL)
    {
       std::cout<<"coucou je suis une erreur de chargement de texture !!! "<<std::cout;
    }
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ptr_texture->getWidth(), ptr_texture->getHeight(), 0, GL_RGBA, GL_FLOAT, ptr_texture->getPixels());

       //glEnable(GL_BLEND);
       //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

       if(confData.getMipMapFilter())
       {
           glEnable(GL_TEXTURE_2D);
           glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
       }
       else
       {
           if(confData.getMinFilter()==0)
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
           else if(confData.getMinFilter()==1)
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

           if(confData.getMaxFilter()==0)
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
           if(confData.getMaxFilter()==1)
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       }




    glBindTexture(GL_TEXTURE_2D, 0);

    return  textureId;
}

GLuint LoadImageFromFile(std::string path_string)
{
    std::unique_ptr<Image> ptr_texture = loadImage(path_string.c_str());

    if(ptr_texture ==NULL)
    {
       std::cout<<"coucou je suis une erreur de chargement de texture !!! "<<std::cout;
    }
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ptr_texture->getWidth(), ptr_texture->getHeight(), 0, GL_RGBA, GL_FLOAT, ptr_texture->getPixels());

       //glEnable(GL_BLEND);
       //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);





    glBindTexture(GL_TEXTURE_2D, 0);

    return  textureId;
}*/

