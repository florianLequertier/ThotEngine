
#include <iostream>

#include "thotEngine/World.hpp"
#include "thotEngine/Component.hpp"
#include "thotEngine/Entity.hpp"

int main(int argc, char** argv)
{
    te::World world;
    world.init();

    auto entity01 = world.InstantiateNew<te::Entity>();
    entity01->setName("toto");

    entity01 = world.InstantiateNew<te::Entity>();
    entity01->setName("tutu");
    world.attachTo<te::Component>(entity01);

    world.destroy(entity01);

    for(int i = 0; i < 10; ++i)
    {
        world.update();
    }

    return 0;
}
