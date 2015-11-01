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
public :
    World();
    ~World();
    template<typename T>
    Handler<T> InstantiateNew();
    template<typename T>
    Handler<T> Instantiate(T model);
    template<typename T>
    void destroy(Handler<T> target);

    void update();

private:
    CMap m_content;

    TestSystem m_testSystem;

    std::shared_ptr<CArray<Component>> m_ptrToComponents;

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

}

#endif // WORLD_HPP
