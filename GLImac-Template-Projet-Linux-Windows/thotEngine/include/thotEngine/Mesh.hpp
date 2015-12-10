#ifndef MESH_HPP
#define MESH_HPP

#include <stdlib.h>
#include <stdio.h>
#include <memory>

#include <SDL\SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include "GLEntity.hpp"

#define INVALID_MATERIAL 0xFFFFFFFF

namespace te
{

class Mesh
{
private:
    bool initFromScene(const aiScene* pScene, const std::string& Filename);
    void initMesh(unsigned int Index, const aiMesh* paiMesh);
    void clear();


    std::string m_name;

    std::vector<GLEntity> m_entities;
    //std::shared_ptr<Material> m_material;

    glm::vec3 m_upperRight;
    glm::vec3 m_lowerLeft;

public:

    Mesh(std::string name = "Mesh");
    ~Mesh();

    void init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices/*, std::shared_ptr<Material> material*/);
    bool load(const std::string& path); // version without engine check

    void pushToGPU(); // envoie les donn�es stock�es dans la m�moire de l'ordinateur � la m�moire de la CG
    void popFromGPU();

    //std::shared_ptr<Material> getMaterial() const;
    //void setMaterial(std::shared_ptr<Material> material);

    void draw();

    glm::vec3 getUpperRight() const;
    glm::vec3 getLowerLeft() const;
};


}

#endif // MESH_HPP

