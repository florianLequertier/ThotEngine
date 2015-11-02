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

ResourceManager::ResourceManager(std::string applicationPath) : m_applicationPath(applicationPath)
{

}

ResourceManager::~ResourceManager()
{
    m_meshes.clear();
    //m_meshesMapping.clear();

    m_images.clear();
    //m_textureMapping.clear();
}

std::shared_ptr<Mesh> ResourceManager::loadMesh(std::string name, std::string path)
{
    if (m_meshes.find(name) != m_meshes.end())
    {
        return m_meshes[name];
    }
    else
    {
        auto newMesh = std::shared_ptr<Mesh>(new Mesh(name));

        newMesh->load(*this, path);
        m_meshes[name] = newMesh;
        //m_meshesMapping[name] = m_meshes.size() - 1;

        return newMesh;
    }
}

std::shared_ptr<Mesh> ResourceManager::loadMesh(std::string name, const std::vector<gl::Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    if (m_meshes.find(name) != m_meshes.end())
    {
        return m_meshes[name];
    }
    else
    {
        auto newMesh = std::shared_ptr<Mesh>(new Mesh(name));
        newMesh->init(vertices, indices);
        m_meshes[name] = newMesh;
        //m_meshesMapping[name] = m_meshes.size() - 1;

        return newMesh;
    }
}

std::shared_ptr<Image> ResourceManager::loadImage(std::string name, std::string path)
{
    if (m_textures.find(name) != m_images.end())
    {
        return m_images[name];
    }
    else
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

        m_images[name] = pImage;

        return pImage;
    }
}

std::shared_ptr<GLProgram> ResourceManager::loadProgram(std::string name, std::string vsRelativePath, std::string fsRelativePath)
{
    if (m_programs.find(name) != m_programs.end())
    {
        return m_programs[name];
    }
    else
    {
        auto newProgram = std::make_shared<GLProgram>(m_applicationPath, vsRelativePath, fsRelativePath);
        newProgram->setProgramName(name);
        m_programs[name] = newProgram;
        return newProgram;
    }
}

std::string ResourceManager::getApplicationPath() const
{
    return m_applicationPath;
}

std::shared_ptr<Material> ResourceManager::createMaterial(std::string programName)
{
    if (m_programs.find(programName) == m_programs.end())
    {
        std::cout << "error when creating material with name : " << programName << " there is no program with this name" << std::endl;
        return nullptr;
    }

    auto newMat = std::shared_ptr<Material>(new Material(m_programs[programName]));
    return newMat;
}

std::shared_ptr<Material> ResourceManager::createMaterial(std::string programName, std::vector<std::shared_ptr<gl::Image> > textures)
{
    if (m_programs.find(programName) == m_programs.end())
    {
        std::cout << "error when creating material with name : " << programName << " there is no program with this name" << std::endl;
        return nullptr;
    }

    auto newMat = std::shared_ptr<Material>(new Material(m_programs[programName], textures));
    return newMat;
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

