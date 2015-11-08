
#include "thotEngine/CArray.hpp"

namespace te{

    Handler::Handler(): m_index(-1), m_valid(false), m_type(typeid(WorldObject))
    {

    }

    Handler::Handler(std::type_index type, int index): m_index(index), m_valid(true), m_type(type)
    {

    }

}

