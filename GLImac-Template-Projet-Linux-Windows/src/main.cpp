
#include <iostream>

#include "thotEngine/World.hpp"
#include "thotEngine/Component.hpp"

int main(int argc, char** argv)
{
    te::World world;

    auto component = world.InstantiateNew<te::Component>();
    component->setName("toto");

    component = world.InstantiateNew<te::Component>();
    component->setName("tutu");

    world.destroy(component);

    for(int i = 0; i < 10; ++i)
    {
        world.update();
    }

    return 0;
}
