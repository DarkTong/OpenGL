#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>    //OpenGL头文件

class Shader
{
    public:
        // 程序ID
        GLint Program;

        // 构造器读取并构造着色器
        Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
        // 析构函数
        virtual ~Shader();
        // 使用程序
        void Use();
    protected:

    private:
};

#endif // SHADER_H
