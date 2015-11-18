#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "Component.hpp"

namespace te{

class Script : public Component
{
public:
    Script();
    virtual ~Script();

    virtual void start();
    virtual void update();
};

}

#endif // SCRIPT_HPP
