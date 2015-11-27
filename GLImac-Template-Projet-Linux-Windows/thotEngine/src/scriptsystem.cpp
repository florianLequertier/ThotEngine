#include "thotEngine/scriptsystem.hpp"
#include "thotEngine/Script.hpp"

//forward
#include "thotEngine/world.hpp"

namespace te{

void ScriptSystem::init(World& world, std::vector<std::shared_ptr<CArray<Script>>>& scripts)
{
    for(auto script : scripts)
        for(int i = 0; i < script->size(); ++i)
        {
            script->parse(i).init(world);
        }
}

void ScriptSystem::update(std::vector<std::shared_ptr<CArray<Script>>>& scripts)
{
    for(auto script : scripts)
        for(int i = 0; i < script->size(); ++i)
        {
            script->parse(i).update();
        }
}

}
