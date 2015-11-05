#ifndef WORLD_HPP
#define WORLD_HPP

#include <typeinfo>
#include <map>
#include <memory>

#include "Entity.hpp"
#include "component.hpp"
#include "CArray.hpp"

#include "TestSystem.hpp"

#include "MeshRenderer.hpp"
#include "ResourceManager.hpp"
#include "MaterialManager.hpp"

namespace te{

class World
{
private:

    //world content
    CMap m_content;

    //systems
    TestSystem m_testSystem;

    //shortcup to CArrays in the CMap, to improve performance
    std::shared_ptr<CArray<Component>> m_ptrToEntities;
    std::shared_ptr<CArray<MeshRenderer>> m_ptrToMeshRenderers;

    //resources
    std::shared_ptr<ResourceManager> m_resourceManager;
    std::shared_ptr<MaterialManager> m_materialManager;

public :
    World();
    ~World();
    void init();

    void pushToGPU();
    void popFromGPU();

    template<typename T>
    ExternalHandler<T> InstantiateNew();
    template<typename T>
    ExternalHandler<T> Instantiate(T model);
    template<typename T>
    void destroy(ExternalHandler<T> target);

    template<typename T>
    ExternalHandler<T> attachTo(ExternalHandler<Entity> entity);

    template<typename T>
    ExternalHandler<T> removeFrom(ExternalHandler<Entity> entity);

    template<typename T>
    ExternalHandler<T> makeExternal(const Handler& handler);

    void update();

};

template<typename T>
ExternalHandler<T> World::InstantiateNew()
{
    int index = std::static_pointer_cast<CArray<T>>(m_content[typeid(T)])->instantiate();
    return ExternalHandler<T>(&m_content, index);
}

template<typename T>
ExternalHandler<T> World::Instantiate(T model)
{
    int index = std::static_pointer_cast<CArray<T>>(m_content[typeid(T)])->instantiate(model);
    return ExternalHandler<T>(&m_content, index);
}

template<typename T>
void World::destroy(ExternalHandler<T> target)
{
    std::static_pointer_cast<CArray<T>>(m_content[typeid(T)])->remove(target);
}

template<typename T>
ExternalHandler<T> World::attachTo(ExternalHandler<Entity> entity)
{
    auto newComponent = InstantiateNew<T>();
    entity->addComponent<T>(newComponent);
    return newComponent;
}

template<typename T>
ExternalHandler<T> World::removeFrom(ExternalHandler<Entity> entity)
{
    auto removed = entity->removeComponent<T>();
    destroy(removed);
}

template<typename T>
ExternalHandler<T> World::makeExternal(const Handler& handler)
{
    return ExternalHandler<T>(handler, m_content);
}

}

#endif // WORLD_HPP
