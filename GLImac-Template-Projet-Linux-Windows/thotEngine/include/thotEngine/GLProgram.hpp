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
    enum ShaderType{VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER};

    std::string m_name;
    GLuint m_programID;

public:

    GLProgram();
    GLProgram(const std::string& name, const std::string& vsPath, const std::string& fsPath);
    GLProgram(const std::string& name, const std::string& shaderPath, ShaderType shaderType);
    void init(const std::string& name, const std::string& shaderPath, ShaderType shaderType);
    void init(const std::string& name, const std::string& vsPath, const std::string& fsPath);
    virtual ~GLProgram();

    void use();

    std::string getProgramName() const;
    void setProgramName(std::string name);

    GLuint getId() const;

    GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);
    GLuint loadShader(const std::string& shader_file_path, ShaderType type);
};



}

#endif // GLPROGRAM_HPP
