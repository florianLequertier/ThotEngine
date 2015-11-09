#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "Component.hpp"

namespace te{

class Script : Component
{
public:
    virtual void init() = 0;
    virtual void update() = 0;
};

}

#endif // SCRIPT_HPP
