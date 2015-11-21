#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <memory>
#include "GLProgram.hpp"
#include "Image.hpp"
#include "ResourceManager.hpp"
#include "light.hpp"
#include "cubemap.hpp"

namespace te{

class Material
{
protected:

    //std::weak_ptr<ResourceManager> m_resourceManager;

    std::weak_ptr<GLProgram> m_program;
    std::vector<std::weak_ptr<Image>> m_images;
    std::vector<GLuint> m_uniforms;

public:
    Material( std::shared_ptr<GLProgram> program );
    Material( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images );
    Material( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images, std::vector<float> parameters);
    virtual ~Material();

    virtual void pushToGPU();
    virtual void popFromGPU();

    GLuint getGlId() const;

    virtual void initUniforms() = 0;
    virtual void setUniforms(const glm::mat4& modelMat, const glm::mat4& projectionMat, const glm::mat4& viewMat) = 0;
    virtual void setUniforms(const glm::mat4& modelMat, const glm::mat4& projectionMat, const glm::mat4& viewMat, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights, const glm::vec3& viewPosition) = 0;

    void use();

};

class UnlitMaterial : public Material
{
private:


public:
    UnlitMaterial( std::shared_ptr<GLProgram> program );
    UnlitMaterial( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images );
    UnlitMaterial( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images, std::vector<float> parameters);
    virtual ~UnlitMaterial();

    virtual void initUniforms() override;
    virtual void setUniforms(const glm::mat4& modelMat, const glm::mat4& projectionMat, const glm::mat4& viewMat) override;
    virtual void setUniforms(const glm::mat4& modelMat, const glm::mat4& projectionMat, const glm::mat4& viewMat, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights, const glm::vec3& viewPosition) override;

};

class LitMaterial : public Material
{
private:
    float m_specularValue;
    float m_shininessValue;

public:
    LitMaterial( std::shared_ptr<GLProgram> program );
    LitMaterial( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images );
    LitMaterial(std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images, std::vector<float> parameters);
    virtual ~LitMaterial();

    virtual void initUniforms() override;
    virtual void setUniforms(const glm::mat4& modelMat, const glm::mat4& projectionMat, const glm::mat4& viewMat) override;
    virtual void setUniforms(const glm::mat4& modelMat, const glm::mat4& projectionMat, const glm::mat4& viewMat, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights, const glm::vec3& viewPosition) override;

};

class SkyboxMaterial : public Material
{
private:
    std::vector<std::weak_ptr<CubeMap>> m_cubeMaps;
public:
    SkyboxMaterial( std::shared_ptr<GLProgram> program );
    SkyboxMaterial( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<CubeMap>> cubeMaps );
    SkyboxMaterial(std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<CubeMap>> cubeMaps, std::vector<float> parameters);

    SkyboxMaterial( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images);
    SkyboxMaterial( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images, std::vector<float> parameters);

    virtual ~SkyboxMaterial();

    virtual void pushToGPU() override;
    virtual void popFromGPU() override;

    virtual void initUniforms() override;
    virtual void setUniforms(const glm::mat4& modelMat, const glm::mat4& projectionMat, const glm::mat4& viewMat) override;
    virtual void setUniforms(const glm::mat4& modelMat, const glm::mat4& projectionMat, const glm::mat4& viewMat, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights, const glm::vec3& viewPosition) override;

};

}

#endif // MATERIAL_HPP
