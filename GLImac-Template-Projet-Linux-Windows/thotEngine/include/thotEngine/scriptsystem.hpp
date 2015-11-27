#ifndef SCRIPTSYSTEM_HPP
#define SCRIPTSYSTEM_HPP

#include "CArray.hpp"

namespace te{

//forward
class World;
class Script;

class ScriptSystem
{
public :
    void init(World& world, std::vector<std::shared_ptr<CArray<Script> > >& scripts);
    void update(std::vector<std::shared_ptr<CArray<Script> > >& scripts);
};

}

#endif // SCRIPTSYSTEM_HPP

