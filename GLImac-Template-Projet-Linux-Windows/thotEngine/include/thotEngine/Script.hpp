#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "Component.hpp"

namespace te{

class Script : public Component
{
public:
    virtual void start();
    virtual void update();
};

}

#endif // SCRIPT_HPP
