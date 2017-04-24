#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// SOIL
#include <SOIL.h>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//myShader
#include "Shader.h"

//using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// 颜色参数
double red=0.1, green=0.3, bule=0.3;

// 定点数量
GLint numberOfVertex=6;

// 两图片的显示比例
GLfloat value;

// triangle & color
GLfloat vertices[]=
{
    //position          //color             //texture position
    0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f
};
GLuint indices[]=
{
    0, 1, 2,    //第一个三角形
    0, 2, 3,    //第二个三角形
    0, 1, 2,
    0, 2, 3
};
// Function prototypes
void init();
int buildWindow(GLFWwindow *, const int &width, const int &height);
void key_callback(GLFWwindow *, GLint,  GLint, GLint, GLint);
void changeRGB();
void testGlmFunction();

int main()
{
    /* test Glm Function */
    testGlmFunction();

    /* test the number of Vertex Attribute */
    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maxinum nr of vertex attributes supported: " << nrAttributes << std::endl;

    /* 初始化环境 */
    init();
    /* 新建第一个窗口 */
    // 获取新建的窗口指针
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if(buildWindow(window, WIDTH, HEIGHT) == -1)
        return -1;

    /* 1.创建顶点着色器和线段着色器 */
    Shader shaderProgram("./movePosition.vecS", "./fragmentShader.fs");

    /* 3.创建顶点缓存对象,顶点数组对象 */
    GLuint VBO; //Vertex Buffer Objects
    GLuint VAO; //Vertex Array Object
    GLuint EBO; //Element Buffer Object
    // 3.1申请缓存
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    // 3.2绑定
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //索引数组
    // 3.3把顶点数组复制到缓冲中供OpenGL使用
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);
    // 3.4设置顶点指针属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); //第一个参数对应 layout的location
    glEnableVertexAttribArray(0);
    // 3.4.1设置颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // 3.4.2设置纹理属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    // 3.4解绑VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* 纹理 */
    // 1.利用SOUL加载图片
    int width1, height1;
    unsigned char* image = SOIL_load_image("container.jpg", &width1, &height1, 0, SOIL_LOAD_RGB);
    std::cout << "sizeof jpg:" << sizeof(image) << std::endl;
    // 2.获取纹理对象
    GLuint texture;
    glGenTextures(1, &texture);
    // 3.绑定纹理
    glBindTexture(GL_TEXTURE_2D, texture);
    // 4.生成纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    // 5.解绑
    glBindTexture(GL_TEXTURE_2D, 0);
    // 6.释放图片内存
    SOIL_free_image_data(image);

    int width2, height2;
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    unsigned char* image1 = SOIL_load_image("awesomeface.png", &width2, &height2, 0, SOIL_LOAD_RGB);
    std::cout << "sizeof png:" << sizeof(image1) << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
    glGenerateMipmap(GL_TEXTURE_2D);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image1);
    /* 纹理结束 */

    /* 构造旋转90度并缩小一半的变换矩阵 */
    // 一个4×4的单位矩阵
    glm::mat4 trans, trans1;
    // 向量已z为轴旋转
    //trans = glm::rotate(trans, 90.0f, glm::vec3(0.0, 0.0, 1.0));
    // 向量缩小一倍
    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));


    /* 画图（渲染） */
    // 为了使画图程序不会在执行完后立刻退出
//    GLfloat shift = 0.0;
    value = 0.5;

    // 减缓变换的时间
    GLfloat ptime=0, ttime=0;
    while(!glfwWindowShouldClose(window)) // 检查GLFW是否被要求退出
    {
        // 检查事件， 检查有没有触发什么时间（键盘输入和鼠标移动）
        glfwPollEvents();

        // 渲染指令， 创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了。
        glClearColor(red, green, bule, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //changeRGB();

        // 绑定
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  //      glBindTexture(GL_TEXTURE_2D, texture);

        // 渲染指令，绘制物体
        shaderProgram.Use();   //加载着色器程序

        // 加载纹理单元到着色器程序
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shaderProgram.Program, "ourTexture"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(shaderProgram.Program, "ourTexture1"), 1);
        glUniform1f(glGetUniformLocation(shaderProgram.Program, "value"), value);
        // 自动旋转矩阵（根据时间参数）
        ttime = (GLfloat)glfwGetTime();
        // 判断变换时机
        if(ttime - ptime >0.1f)
        {
            //ptime = ttime;
            // 旋转矩阵
            trans = glm::rotate(trans, (GLfloat)ttime, glm::vec3(0.0f, 0.0f, 1.0f));
            // 位移矩阵
            trans = glm::translate(trans, glm::vec3(0.001f, 0.001f, 0.0f));
        }
        // 画图
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.Program, "trans"), 1, GL_FALSE, glm::value_ptr(trans));
        glDrawElements(GL_TRIANGLES, numberOfVertex, GL_UNSIGNED_INT, 0);

        // 判断变换时机
        if(ttime - ptime >0.1f)
        {
            ptime = ttime;
            //trans = glm::rotate(trans, (GLfloat)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            // 位移矩阵
            trans1 = glm::translate(trans1, glm::vec3(-0.001f, -0.001f, 0.0f));
            // 缩放矩阵
            trans1 = glm::scale(trans1, glm::vec3(0.999f, 0.999f, 0.1f));
        }
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.Program, "trans"), 1, GL_FALSE, glm::value_ptr(trans1));
        glDrawElements(GL_TRIANGLES, numberOfVertex, GL_UNSIGNED_INT, 0);

        // 解绑
//        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // 交换缓存，增强视觉效果
        glfwSwapBuffers(window);

        // 根据用户输入去做相应的处理
        glfwSetKeyCallback(window, key_callback);
    }
    // 结束后正确释放之前分配的所有资源
    glfwTerminate();
    return 0;
}

//初始化
 void init()
 {
    /* 初始化环境 */
    // 初始化GLFW
    glfwInit();
    // 设置版本信息
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 利用核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // 窗口大小不改变
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
 }

// 新建窗口
int buildWindow(GLFWwindow *window, const int &WIDTH, const int &HEIGHT)
{
    /* 新建第一个窗口 */

    if(window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // ?创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了。
    glfwMakeContextCurrent(window);

    // 为了GLEW在管理OpenGL和函数指针时更多地使用现代化技术
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    int width, height;
    // 设置窗口大小参数
    glfwGetFramebufferSize(window, &width, &height);

    // 左上角坐标和右下角坐标
    glViewport(0, 0, width, height);

    return true;
}

// 回调函数（操作），根据按键去操作窗口
void key_callback(GLFWwindow *window, GLint key,  GLint scancode, GLint action, GLint mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        value += 0.05f;
        if(value >= 1.0f)
            value = 1.0f;
    }
    else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        value -= 0.05;
        if(value <= 0.0f)
            value = 0.0f;
    }
    else if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {

    }
}

// 渲染操作函数
void changeRGB()
{
    red += 0.0001;
    green += 0.0002;
    bule += 0.00001;
    if(red > 0.7) red -= 0.6;
    if(green > 0.6) green -= 0.3;
    if(bule > 0.9) bule -= 0.6;
}

// 测试glm的函数
void testGlmFunction()
{
    // 测试向量位移
    glm::vec4 vec(1.0, 0.0, 0.0, 1.0);
    glm::mat4 trans;
    trans = glm::translate(trans, glm::vec3(1.0, 1.0, 0.0));
    vec = trans*vec;
    std::cout << '(' << vec.x << ',' << vec.y << ',' << vec.z << ')' << std::endl;
}






