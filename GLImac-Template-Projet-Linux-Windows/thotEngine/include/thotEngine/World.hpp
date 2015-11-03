#ifndef WORLD_HPP
#define WORLD_HPP

#include <typeinfo>
#include <map>
#include <memory>

#include "Entity.hpp"
#include "component.hpp"
#include "CArray.hpp"

#include "TestSystem.hpp"

namespace te{

class World
{
private:
    CMap m_content;

    TestSystem m_testSystem;

    //shortcup to CArrays in the CMap, to improve performance
    std::shared_ptr<CArray<Component>> m_ptrToEntities;

public :
    World();
    ~World();
    void init();

    template<typename T>
    Handler<T> InstantiateNew();
    template<typename T>
    Handler<T> Instantiate(T model);
    template<typename T>
    void destroy(Handler<T> target);

    template<typename T>
    Handler<T> attachTo(Handler<Entity> entity);

    void update();

};

template<typename T>
Handler<T> World::InstantiateNew()
{
    int index = std::static_pointer_cast<CArray<T>>(m_content[typeid(T)])->instantiate();
    return Handler<T>(&m_content, index);
}

template<typename T>
Handler<T> World::Instantiate(T model)
{
    int index = std::static_pointer_cast<CArray<T>>(m_content[typeid(T)])->instantiate(model);
    return Handler<T>(&m_content, index);
}

template<typename T>
void World::destroy(Handler<T> target)
{
    std::static_pointer_cast<CArray<T>>(m_content[typeid(T)])->remove(target);
}

template<typename T>
Handler<T> World::attachTo(Handler<Entity> entity)
{
    auto newComponent = InstantiateNew<T>();
    entity->addComponent<T>(newComponent);
    return newComponent;
}

}

#endif // WORLD_HPP
