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
GLfloat value, value1, value2;

// 局部空间转世界空间的转换矩阵
glm::mat4 model[11];
// 世界空间转观察空间的转换矩阵
glm::mat4 view;
// 观察空间转裁剪空间的转换矩阵
glm::mat4 projection;

// 可调节角度
GLfloat xDegree, yDegree, zDegree;

// 构造立方体的36个顶点
GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
/*
// triangle & color
GLfloat vertices[]=
{
    //position          //color             //texture position
    0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f
};
*/
glm::vec3 cubePosition[]
{
    glm::vec3( 0.0f,  0.0f,  -3.0f),
    glm::vec3( 2.0f,  1.0f, -9.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    /*
    glm::vec3(0.2, 0.1, -0.6),
    glm::vec3(0.4, 0.4, -0.4),
    glm::vec3(0.2, -0.3, 0.4),
    glm::vec3(-0.33, -0.2, 0.2),
    glm::vec3(-0.2, 0.1, 0.15),
    */
};

GLuint indices[]=
{
    0, 1, 2, 2, 3, 0,   // 正前
    7, 6, 5, 5, 4, 7,   // 正后
    3, 2, 6, 6, 7, 3,   // 左
    4, 5, 1, 1, 0, 4,   // 右
    4, 0, 3, 3, 7, 4,   // 上
    1, 5, 6, 6, 2, 1,   // 下
};

// Function prototypes
void init();
int buildWindow(GLFWwindow *, const int &width, const int &height);
void key_callback(GLFWwindow *, GLint,  GLint, GLint, GLint);
void changeRGB();

int main()
{
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); //第一个参数对应 layout的location
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // 3.5解绑VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* 纹理 */
    // 加载书柜图片
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

    //加载滑稽脸图片
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
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image1);

    /* 开启深度测试 */
    // 意思：检测深度，那么渲染时便会根据深度选择如何渲染
    glEnable(GL_DEPTH_TEST);

    /* 画图（渲染）*/
    GLfloat ptime[11]={0}, ttime[11]={10};
    bool flag[11] = {0};
    value = 1.0;
    value1 = 1.33;
    value2 = 1.0;
    while(!glfwWindowShouldClose(window)) // 检查GLFW是否被要求退出
    {
        // 检查事件， 检查有没有触发什么时间（键盘输入和鼠标移动）
        glfwPollEvents();

        // 渲染指令， 创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了。（背景色）
        glClearColor(red, green, bule, 0.01f);
        // 清除颜色缓存，清除深度缓存
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //changeRGB();

        // 绑定
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // 设置着色器
        // 渲染指令，绘制物体
        shaderProgram.Use();   //加载着色器程序

        // 获取位置
        GLint ourTextureLoc, ourTextureLoc1, valueLoc,modelLoc, viewLoc, projectionLoc;
        ourTextureLoc = glGetUniformLocation(shaderProgram.Program, "ourTexture");
        ourTextureLoc1 = glGetUniformLocation(shaderProgram.Program, "ourTexture1");
        valueLoc = glGetUniformLocation(shaderProgram.Program, "value");
        viewLoc = glGetUniformLocation(shaderProgram.Program, "view");
        projectionLoc = glGetUniformLocation(shaderProgram.Program, "projection");

        // 观察矩阵
        glm::mat4 tmp;
        view = glm::translate(tmp, glm::vec3(value1, value, value2));

        // 投射投影
        //projection = glm::perspective(value, (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.1f, 100.f);
        projection = glm::perspective(45.0f, 1.3333f, 0.1f, 100.f);

        // 加载图片纹理到着色器程序
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(valueLoc, value);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(ourTextureLoc, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(ourTextureLoc1, 1);


        // 自动旋转矩阵（根据时间参数）
        // 判断变换时机
        for(int i=0;i<5;++i)
        {
            ttime[i] = (GLfloat)glfwGetTime();
            if(ttime[i] - ptime[i] >0.1f)
            {
                // 旋转矩阵-以x为轴
                if(i%3==0)model[i] = glm::rotate(model[i], (GLfloat)(ttime[i]-ptime[i])*10.0f*(i+1), glm::vec3(1.0f*i/1.2, 0.3f*i, 0.5f));
                // 唯一矩阵
                if(!flag[i]) model[i] = glm::translate(model[i], cubePosition[i]);
                flag[i] = 1;
                ptime[i] = ttime[i];
            }
            // 获取位置
            modelLoc = glGetUniformLocation(shaderProgram.Program, "model");
            // 加载纹理单元到着色器程序
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model[i]));
            // 画图
            glDrawArrays(GL_TRIANGLES, 0, 36);
            //glDrawElements(GL_TRIANGLES, numberOfVertex, GL_UNSIGNED_INT, 0);
        }

        // 解绑
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
        value += 1.0f;
        //if(value >= 1.0f)
          //  value = 1.0f;
    }
    else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        value -= 1.0f;
        //if(value <= 0.0f)
         //   value = 0.0f;
    }
    else if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        value1 += 0.5f;
    }
    else if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        value1 -= 0.5;
    }
    else if(key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        value2 -= 0.5;
    }
    else if(key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        value2 += 0.5;
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






