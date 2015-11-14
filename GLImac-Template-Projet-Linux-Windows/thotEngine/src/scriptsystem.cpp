#include "thotEngine/scriptsystem.hpp"
#include "thotEngine/Script.hpp"

namespace te{

void ScriptSystem::init(std::vector<std::shared_ptr<CArray<Script>>>& scripts)
{
    for(auto script : scripts)
        for(int i = 0; i < script->size(); ++i)
        {
            script->parse(i).start();
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
