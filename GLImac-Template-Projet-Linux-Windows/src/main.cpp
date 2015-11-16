#include <memory>
#include <iostream>

#include <GL/glew.h>
#include "glm/glm.hpp"

#include "thotEngine/World.hpp"
#include "thotEngine/Component.hpp"
#include "thotEngine/Entity.hpp"
#include "thotEngine/prefab.hpp"
#include "thotEngine/ResourceManager.hpp"
#include "thotEngine/MaterialManager.hpp"
#include "thotEngine/prefabmanager.hpp"

#include "thotEngine/WindowManager.hpp"
#include "thotEngine/Input.hpp"

//scripts
# include "thotEngine/FreeFlyCam.hpp"

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

int main(int argc, char** argv)
{

    // Initialize SDL and open a window
    te::WindowManager windowManager(WINDOW_WIDTH, WINDOW_HEIGHT, "ThotEngine");

    SDL_EnableKeyRepeat(10,10);

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
    resourceManager.loadImage("skybox_tex_dif", "assets/textures/skybox_texture.png");
    resourceManager.loadMesh("ship", "assets/models/cube.obj");
    resourceManager.loadProgram("glProg_3D", "shaders/3D.vs.glsl", "shaders/3D.fs.glsl");
    resourceManager.loadProgram("glProg_skybox", "shaders/skyShader.vs.glsl", "shaders/skyShader.fs.glsl");
    resourceManager.loadProgram("glProg_3DLight", "shaders/3DLight.vs.glsl", "shaders/3DLight.fs.glsl");

    //make materials
    te::MaterialManager& materialManager = te::MaterialManager::getInstance();
    materialManager.createMaterial<te::UnlitMaterial>("ship_mat", "glProg_3D", {"ship_tex_dif"});
    materialManager.createMaterial<te::UnlitMaterial>("skybox_mat", "glProg_skybox", {"skybox_tex_dif"});
    materialManager.createMaterial<te::LitMaterial>("ship02_mat", "glProg_3DLight", {"ship_tex_dif"}, {1,100});

    //create world
    te::World world;

    //make a prefab

    //prefab 01
    std::shared_ptr<te::Prefab> prefab01 = std::make_shared<te::Prefab>("prefab01");
    prefab01->setMakeFunction([](te::ExternalHandler<te::Entity> entity, te::World& world){

        entity->setName("prefab01");
        entity->addComponent<te::Transform>(world)->setTranslation(0,0,0);
        auto prefabComponentHandler = entity->addComponent<te::MeshRenderer>(world);
        prefabComponentHandler->setMaterial("ship_mat");
        prefabComponentHandler->setMesh("ship");

    });

    te::PrefabManager::getInstance().add(prefab01);

    //populate world

    //ship 01
    auto entityHandler = world.instantiate();
    entityHandler->setName("ship01");
    entityHandler->addComponent<te::Transform>(world)->setTranslation(0,0,10);
    auto componentHandler = entityHandler->addComponent<te::MeshRenderer>(world);
    componentHandler->setMaterial("ship_mat");
    componentHandler->setMesh("ship");

    //test 01
    entityHandler = world.instantiate();
    entityHandler->setName("test01");
    entityHandler->addComponent<te::Transform>(world);
    auto cameraHandler = entityHandler->addComponent<te::Camera>(world);
    cameraHandler->setSkyboxMaterial("skybox_mat");
    cameraHandler->setUseSkybox(true);
    entityHandler->addComponent<te::FreeFlyCam>(world);

    //test 02
    entityHandler = world.instantiate();
    entityHandler->setName("ship02");
    entityHandler->addComponent<te::Transform>(world)->setTranslation(0,10,10);
    componentHandler = entityHandler->addComponent<te::MeshRenderer>(world);
    componentHandler->setMaterial("ship_mat");
    componentHandler->setMesh("ship");

    //test 03
    entityHandler = world.instantiate(prefab01);

    //test 04
    entityHandler = world.instantiate("prefab01");
    entityHandler->getComponent<te::Transform>()->setTranslation(10,0,0);

//    world.destroy(entityHandler);

    //finalize openGL context
    world.pushToGPU();

    //init world
    world.init();

    //main loop
    bool done = false;
    while(!done)
    {
        // Event loop:
        SDL_Event event;

        while(te::Input::pollEvent(event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    done = true; // Leave the loop after this iteration
                break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                break;
            }
        }

        //updates
        world.update();

        //render
        world.render();
        windowManager.swapBuffers();

    }

    return 0;
}
