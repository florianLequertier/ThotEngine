#ifndef GLPROGRAM_HPP
#define GLPROGRAM_HPP


#include <iostream>
#include <cstdlib>
#include <vector>
#include <functional>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace te{

class GLProgram
{
protected :

    enum UNIFORM_NAME{MVP, MV, NORMAL, DIFFUSE};

    std::string m_name;
    GLuint m_programID;

public:

    GLProgram(const std::string& name, const std::string& vsPath, const std::string& fsPath);
    virtual ~GLProgram();

    void use();

    std::string getProgramName() const;
    void setProgramName(std::string name);

    GLuint getId() const;
};

GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);

}

#endif // GLPROGRAM_HPP
