
#include <memory>
#include <iostream>

#include "thotEngine/World.hpp"
#include "thotEngine/Component.hpp"
#include "thotEngine/Entity.hpp"
#include "thotEngine/ResourceManager.hpp"
#include "thotEngine/MaterialManager.hpp"

int main(int argc, char** argv)
{


    //load resources
    te::ResourceManager& resourceManager = te::ResourceManager::getInstance();
    resourceManager.loadImage("ship_tex_dif", "assets/images/ship/ship_tex_dif.png");
    resourceManager.loadMesh("ship", "assets/meshes/ship.obj");
    resourceManager.loadProgram("glProg_simple3D", "shaders/simple3D.vs.glsl", "shaders/simple3D.fs.glsl");

    //make materials
    te::MaterialManager& materialManager = te::MaterialManager::getInstance();
    materialManager.createMaterial("ship_mat", "glProg_simpleMat", {"ship_tex_dif"});

    //create world
    te::World world;
    world.init();

    //populate world

    //ship 01
    auto entityHandler = world.InstantiateNew<te::Entity>();
    entityHandler->setName("ship01");
    entityHandler->addComponent<te::Transform>(world);
    auto componentHandler = entityHandler->addComponent<te::MeshRenderer>(world);
    componentHandler->setMaterial("ship_mat");
    componentHandler->setMesh("ship");

    //test 01
    entityHandler = world.InstantiateNew<te::Entity>();
    entityHandler->setName("test01");
    world.attachTo<te::Transform>(entityHandler);

    //test 02
    entityHandler = world.InstantiateNew<te::Entity>();
    entityHandler->setName("test02");
    world.attachTo<te::Transform>(entityHandler);

    world.destroy(entityHandler);

    //finalize openGL context
    world.pushToGPU();

    //main loop
    for(int i = 0; i < 10; ++i)
    {
        world.update();
    }

    return 0;
}
