#include "Shader.h"

// 构造器读取并构造着色器
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
{
    /**读取着色器源文件并构造着色器
      */
    // 1.从文件路劲中获取定点/片段着色器
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    // 保证ifstream对象可以抛出异常
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::badbit);
    try
    {
        std::stringstream vShaderStream, fShaderStream, gShaderStream;

        // 打开文件 - 读取文件内容 - 关闭 -转成string
        vShaderFile.open(vertexPath);
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();
        vertexCode = vShaderStream.str();

        fShaderFile.open(fragmentPath);
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();
        fragmentCode = fShaderStream.str();

        gShaderFile.open(geometryPath);
        gShaderStream << gShaderFile.rdbuf();
        gShaderFile.close();
        geometryCode = gShaderStream.str();

    }
    catch(std::ifstream::failure e)
    {
        //      std::cout << "ERROR:SHADER::FILE_NOTSUCCESSFULLY_READ" << std::cout;
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    const GLchar* gShaderCode = geometryCode.c_str();

    // 2.编译和着色器
    GLint vertex, fragment, geometry;
    GLint success;
    GLchar infoLog[512];

    // 顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // 判断是否编译错误，有则输出
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR:VERTEX COMPILE FAILED\n" << infoLog << std::endl;
    }

    // 片段着色器
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // 判断是否编译错误，有则输出
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR:FRAGMENT　COMPILE FAILED\n" << infoLog << std::endl;
    }

    // 几何着色器
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    // 判断是否编译错误，有则输出
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(geometry, 512, NULL, infoLog);
        std::cout << "ERROR:GEOMETRY　COMPILE FAILED\n" << infoLog << std::endl;
    }

    // 3.着色器程序链接
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    //glAttachShader(this->Program, geometry);
    glLinkProgram(this->Program);
    // 打印链接错误
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 4.删除着色器由于已经链接到程序了
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(geometry);
}

// 析构函数
Shader::~Shader()
{

}

// 使用程序
void Shader::Use()
{
    glUseProgram(this->Program);
}
