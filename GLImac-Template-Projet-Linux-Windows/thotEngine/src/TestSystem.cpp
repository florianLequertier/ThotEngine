#include <memory>
#include <iostream>

#include "thotEngine/TestSystem.hpp"

namespace te {

TestSystem::TestSystem()
{

}

TestSystem::~TestSystem()
{

}

void TestSystem::update(std::shared_ptr<CArray<Entity> > entities)
{
    //auto container = std::static_pointer_cast<CArray<Component>>(components);
    for(int i = 0; i < entities->size(); ++i)
    {
        if(entities->isActive(i))
        std::cout<<"component : "<<entities->operator[](i).getName()<<std::endl;
    }
}

}
