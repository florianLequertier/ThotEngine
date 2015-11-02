
#include <cstdlib>
#include <string>
#include "thotEngine/GLProgram.hpp"

namespace te{

GLProgram::GLProgram(const std::string& applicationPath, const std::string& name ) : m_name(name)
{
    m_programID = loadShaders(applicationPath + "shaders/3D.vs.glsl", applicationPath + "shaders/3D.fs.glsl");
    std::cout << "program " << std::to_string(m_programID) << applicationPath << "shaders/3D.vs.glsl" << applicationPath << "shaders/3D.fs.glsl" << std::endl;
}

GLProgram::GLProgram(const std::string& applicationPath, const std::string& vsRelativePath, const std::string& fsRelativePath, const std::string& name) : m_name(name)
{
    m_programID = loadShaders(applicationPath + vsRelativePath, applicationPath + fsRelativePath);
    std::cout << "program : " << std::to_string(m_programID) << " _vs : " << applicationPath << vsRelativePath << " _fs : " << applicationPath << fsRelativePath << std::endl;
}

GLProgram::GLProgram(const std::string& vsPath, const std::string& fsPath, const std::string& name) : m_name(name)
{
    m_programID = loadShaders(vsPath, fsPath);
    std::cout << "program " << std::to_string(m_programID) << vsPath << fsPath << std::endl;
}

GLProgram::~GLProgram()
{
    m_uniforms.clear();
    glDeleteProgram(m_programID);
}

void GLProgram::use()
{
    glUseProgram(m_programID);
}

std::string GLProgram::getProgramName() const
{
    return m_name;
}

void GLProgram::setProgramName(std::string name)
{
    m_name = name;
}

GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path)
{

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open())
    {
        std::string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    else
    {
        std::cerr << "can't open : " << vertex_file_path << std::endl;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open()){
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
    else
    {
        std::cerr << "can't open : " << fragment_file_path << std::endl;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    std::cout<<"Compiling shader : "<< vertex_file_path<<std::endl;
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    std::cout<< &VertexShaderErrorMessage[0] <<std::endl;

    // Compile Fragment Shader
    std::cout<<"Compiling shader : "<<fragment_file_path;
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    std::cout<< &FragmentShaderErrorMessage[0] <<std::endl;

    // Link the program
    std::cout<<"Linking program"<<std::endl;
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    std::cout<< &ProgramErrorMessage[0] <<std::endl;

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

}
