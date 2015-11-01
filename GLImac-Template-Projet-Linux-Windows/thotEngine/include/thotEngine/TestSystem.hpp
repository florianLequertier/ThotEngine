#ifndef TESTSYSTEM_HPP
#define TESTSYSTEM_HPP

#include "thotEngine/CArray.hpp"
#include "thotEngine/Component.hpp"

namespace te{

class TestSystem
{
public :
    TestSystem();
    ~TestSystem();

    void update(std::shared_ptr<CArray<Component>> components);
};

}

#endif // TESTSYSTEM_HPP
