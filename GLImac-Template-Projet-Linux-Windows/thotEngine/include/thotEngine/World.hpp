#ifndef WORLD_HPP
#define WORLD_HPP

#include <typeinfo>
#include <map>
#include <memory>

#include "CArray.hpp"

#include "Camera.hpp"
#include "Script.hpp"
#include "prefab.hpp"

//systems
#include "TestSystem.hpp"
#include "Renderer.hpp"
#include "scriptsystem.hpp"

namespace te{

class World
{
private:

    //world content
    CMap m_content;

    //systems
    TestSystem m_testSystem;
    Renderer m_renderer;
    ScriptSystem m_scriptSystem;

    //shortcup to CArrays in the CMap, to improve performance
    std::shared_ptr<CArray<Entity>> m_ptrToEntities;
    std::shared_ptr<CArray<MeshRenderer>> m_ptrToMeshRenderers;
    std::shared_ptr<CArray<Transform>> m_ptrToTransforms;
    std::shared_ptr<CArray<Camera>> m_ptrToCameras;
    std::vector<std::shared_ptr<CArray<Script>>> m_ptrsToScripts;

    //resources
    //std::shared_ptr<ResourceManager> m_resourceManager;
    //std::shared_ptr<MaterialManager> m_materialManager;

public :
    World();
    ~World();
    void init();

    void pushToGPU();
    void popFromGPU();

    ExternalHandler<Entity> instantiate();
    ExternalHandler<Entity> instantiate(std::shared_ptr<Prefab> prefab);
    ExternalHandler<Entity> instantiate(std::string prefabName);

    template<typename T>
    void destroy(ExternalHandler<T> target);

    template<typename T>
    ExternalHandler<T> attachTo(Handler entity);

    template<typename T>
    ExternalHandler<T> attachTo(Handler entity, std::shared_ptr<T> model);

    template<typename T>
    ExternalHandler<T> removeFrom(Handler entity);

    template<typename T>
    ExternalHandler<T> makeExternal(const Handler& handler);

    void update();
    void render();

private:
    template<typename T>
    ExternalHandler<T> instantiateNew();

    template<typename T>
    ExternalHandler<T> instantiateFrom(T& model);

};

template<typename T>
ExternalHandler<T> World::instantiateNew()
{
    if( m_content.find( typeid(T) ) == m_content.end() )
    {
        m_content[typeid(T)] = std::make_shared< CArray<T> >();

        //put a ref to the new CArray into scriptsPtrs.
        m_ptrsToScripts.push_back( std::static_pointer_cast<CArray<Script>>(m_content[typeid(T)]) );
    }

    int index = std::static_pointer_cast<CArray<T>>(m_content[typeid(T)])->instantiate();
    return ExternalHandler<T>(&m_content, index);
}

template<typename T>
ExternalHandler<T> World::instantiateFrom(T& model)
{
    if( m_content.find( typeid(T) ) != m_content.end() )
        m_content[typeid(T)] = std::make_shared< CArray<T> >();

    int index = std::static_pointer_cast<CArray<T>>(m_content[typeid(T)])->instantiate(model);
    return ExternalHandler<T>(&m_content, index);
}

template<typename T>
void World::destroy(ExternalHandler<T> target)
{
    std::static_pointer_cast<CArray<T>>(m_content[typeid(T)])->remove(target);
}

template<typename T>
ExternalHandler<T> World::attachTo(Handler entity)
{
    auto newComponent = instantiateNew<T>();
    auto tmpExternal = makeExternal<Entity>(entity);
    newComponent->setOwner(tmpExternal);
    tmpExternal->addComponent<T>(newComponent);
    return newComponent;
}

template<typename T>
ExternalHandler<T> World::attachTo(Handler entity, std::shared_ptr<T> model)
{
    auto newComponent = instantiateFrom<T>(*model);
    auto tmpExternal = makeExternal<Entity>(entity);
    newComponent->setOwner(tmpExternal);
    tmpExternal->addComponent<T>(newComponent);
    return newComponent;
}

template<typename T>
ExternalHandler<T> World::removeFrom(Handler entity)
{
    auto removed = makeExternal<Entity>(entity)->removeComponent<T>();
    destroy(removed);
}

template<typename T>
ExternalHandler<T> World::makeExternal(const Handler& handler)
{
    return ExternalHandler<T>(handler, &m_content);
}

}

//forward declaration of Entity
namespace te {


template<typename T>
ExternalHandler<T> Entity::addComponent(World& worldPtr)
{
    return worldPtr.attachTo<T>(thisHandler());
}

template<typename T>
ExternalHandler<T> Entity::removeComponent(World& worldPtr)
{
    return worldPtr.removeFrom<T>(thisHandler());
}


template<typename T>
ExternalHandler<T> Entity::addComponent(World& worldPtr, T& model)
{
    return worldPtr.attachTo<T>(thisHandler(), model);
}

}

#endif // WORLD_HPP
