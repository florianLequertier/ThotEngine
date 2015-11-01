#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <string>

namespace te{

class Component
{
public :
    std::string getName() const;
    void setName(std::string name);
private :
    std::string m_name;
};

}

#endif // COMPONENT_HPP
