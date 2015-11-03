#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <vector>
#include <memory>
#include "thotEngine/CArray.hpp"
#include "thotEngine/Transform.hpp"

namespace te{

class Entity
{
private:
    std::string m_name;

    std::vector<std::shared_ptr<IHandler>> m_components;

public:
    Entity();
    ~Entity();

    std::string getName() const;
    void setName(std::string name);

    template<typename T>
    Handler<T> getComponent();

    template<typename T>
    Handler<T> addComponent(Handler<T> component);
};

template<typename T>
Handler<T> Entity::getComponent()
{
    for(int i = 0; i < m_components.size(); ++i)
    {
        if(m_components[i]->istypeof( typeid(T) ))
        {
            return *std::static_pointer_cast<Handler<T>>(m_components[i]);
        }
    }
}

template<typename T>
Handler<T> Entity::addComponent(Handler<T> component)
{
    m_components.push_back(component);
}

}

#endif // ENTITY_HPP
