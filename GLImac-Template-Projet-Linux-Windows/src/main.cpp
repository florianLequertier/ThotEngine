#include <memory>
#include <iostream>

#include <GL/glew.h>
#include "glm/glm.hpp"

#include "thotEngine/World.hpp"
#include "thotEngine/Component.hpp"
#include "thotEngine/Entity.hpp"
#include "thotEngine/ResourceManager.hpp"
#include "thotEngine/MaterialManager.hpp"

#include "thotEngine/SDLWindowManager.hpp"

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

int main(int argc, char** argv)
{

    // Initialize SDL and open a window
    te::SDLWindowManager windowManager(WINDOW_WIDTH, WINDOW_HEIGHT, "ThotEngine");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;


    //load resources
    te::ResourceManager& resourceManager = te::ResourceManager::getInstance();
    resourceManager.loadImage("ship_tex_dif", "assets/textures/cobble2.png");
    resourceManager.loadMesh("ship", "assets/models/cube.obj");
    resourceManager.loadProgram("glProg_3D", "shaders/3D.vs.glsl", "shaders/3D.fs.glsl");

    //make materials
    te::MaterialManager& materialManager = te::MaterialManager::getInstance();
    materialManager.createMaterial("ship_mat", "glProg_3D", {"ship_tex_dif"});

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
    entityHandler->addComponent<te::Transform>(world);

    //test 02
    entityHandler = world.InstantiateNew<te::Entity>();
    entityHandler->setName("test02");
    entityHandler->addComponent<te::Transform>(world);
    //world.attachTo<te::Transform>(entityHandler);

//    world.destroy(entityHandler);

    //finalize openGL context
    world.pushToGPU();

    bool done = false;

    //main loop
    while(!done)
    {
        // Event loop:
        SDL_Event event;

        while(windowManager.pollEvent(event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    done = true; // Leave the loop after this iteration
                break;
            }
        }

        world.update();

    }

    return 0;
}
