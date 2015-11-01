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

void TestSystem::update(std::shared_ptr<CArray<Component> > components)
{
    //auto container = std::static_pointer_cast<CArray<Component>>(components);
    for(int i = 0; i < components->size(); ++i)
    {
        if(components->isActive(i))
        std::cout<<"component : "<<components->operator[](i).getName()<<std::endl;
    }
}

}
