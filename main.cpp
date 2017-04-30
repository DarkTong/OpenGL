#include <iostream>
#include <cmath>
#include <string>

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
#include "Camer.h"

//using namespace std;

// Window dimensions
const GLuint WIDTH = 1200, HEIGHT = 800;

// 颜色参数
//double red=0.3, green=0.5, bule=0.6;
double red=0.1, green=0.1, bule=0.1;

// 定点数量
GLint numberOfVertex=6;

// 两图片的显示比例
GLfloat value, value1, value2;

// 按键记录
GLboolean keyF[1024]={0};

// 单位矩阵
glm::mat4 uniM4;

// 灯光
glm::vec3 lightColor(1.0, 1.0, 1.0);

/* 创建摄影机对象 */
Camer camer(glm::vec3(-2.0f, -1.0f, 4.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            (float)WIDTH, (float)HEIGHT);

// 构造立方体 + 各面法向量 + 纹理坐标
GLfloat vertices[] = {
    // Positions          // Normals           // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
// 物体位置
glm::vec3 cubePosition[]
{
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
// 光源位置
glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};
//glm::vec3 lightPos(1.2, 1.0f, 2.0f);
/*
GLuint indices[]=
{
    0, 1, 2, 2, 3, 0,   // 正前
    7, 6, 5, 5, 4, 7,   // 正后
    3, 2, 6, 6, 7, 3,   // 左
    4, 5, 1, 1, 0, 4,   // 右
    4, 0, 3, 3, 7, 4,   // 上
    1, 5, 6, 6, 2, 1,   // 下
};
*/


// Function prototypes
void init();
int buildWindow(GLFWwindow *, const int &width, const int &height);
void changeRGB();
void doCamerMovement();
void key_callback(GLFWwindow *, GLint,  GLint, GLint, GLint);
void mouse_callback(GLFWwindow *, double, double);
void scroll_callback(GLFWwindow *, double, double);

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
    Shader shaderProgram("./shader/vecS/objectColor.vs", "./shader/fs/objectColor.fs");
    Shader lightShader("./shader/vecS/lightColor.vs", "./shader/fs/lightColor.fs");

    /* 3.创建顶点缓存对象,顶点数组对象 */
    GLuint VBO; //Vertex Buffer Objects
    GLuint VAO, lightVAO; //Vertex Array Object
    // 3.1 申请缓存 - 绑定 - 复制/设置 - 解绑
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

    // 物体
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // 灯
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glBindBuffer(GL_VERTEX_ARRAY, 0);

    /* 加载纹理图片 */
    GLuint diffuseMap;
    unsigned char * image;
    int width, height;
    glGenTextures(1, &diffuseMap);
    image = SOIL_load_image("./texture/container2.png", &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    /* 加载纹理图片2 */
    GLuint specularMap;
    glGenTextures(1, &specularMap);
    image = SOIL_load_image("./texture/container2_specular.png", &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    /* 加载纹理图片3 */
    /*
    GLuint emissionMap;
    glGenTextures(1, &emissionMap);
    image = SOIL_load_image("./texture/matrix.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, emissionMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    */

    /* 绑定纹理 */
    shaderProgram.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    //glActiveTexture(GL_TEXTURE2);
    //glBindTexture(GL_TEXTURE_2D, emissionMap);

    /* 开启深度测试 */
    // 意思：检测深度，那么渲染时便会根据深度选择如何渲染
    glEnable(GL_DEPTH_TEST);

    // 其他参数
    //GLfloat radius = 2.332;
    //GLfloat curT=0;
    /* 画图（渲染）*/
    while(!glfwWindowShouldClose(window)) // 检查GLFW是否被要求退出
    {
        // 检查事件， 检查有没有触发什么时间（键盘输入和鼠标移动）
        glfwPollEvents();
        // 摄影机移动
        doCamerMovement();

        // 渲染指令， 创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了。（背景色）
        glClearColor(red, green, bule, 0.01f);
        // 清除颜色缓存，清除深度缓存
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //changeRGB();

        // 构造观察空间
        glm::mat4 view = camer.getViewMartix();
        // 构造裁剪空间
        glm::mat4 projection = glm::perspective(camer.camerFov, (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.1f, 300.0f);
        // 移动光源
        //curT = glfwGetTime();
        //lightPos.x = radius * cos(curT);
        //lightPos.z = radius * sin(curT);

        // 物体着色器
        shaderProgram.Use();
        // 配置物体着色器程序参数
        // -配置摄像机参数
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(glGetUniformLocation(shaderProgram.Program, "viewPos"), camer.camerPos.x, camer.camerPos.y, camer.camerPos.z);
        // -配置材质参数
        glUniform3f(glGetUniformLocation(shaderProgram.Program, "material.ambient"), 0.1, 0.1, 0.1);
        glUniform1i(glGetUniformLocation(shaderProgram.Program, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram.Program, "material.specular"), 1);
        glUniform1f(glGetUniformLocation(shaderProgram.Program, "material.shininess"), 32);
        // -配置平行光参数
        glUniform3f(glGetUniformLocation(shaderProgram.Program, "dirLight.direction"), -1.0, -1.0, -1.0);
        glUniform3f(glGetUniformLocation(shaderProgram.Program, "dirLight.ambient"), 0.1, 0.1, 0.1);
        glUniform3f(glGetUniformLocation(shaderProgram.Program, "dirLight.diffuse"), 0.2, 0.2, 0.2);
        glUniform3f(glGetUniformLocation(shaderProgram.Program, "dirLight.specular"), 0.0, 0.0, 0.0);
        // -配置定点光源参数
        for(int i=0;i<4;++i)
        {
            using std::string;
            string th = std::__cxx11::to_string(i);
            string target = "pointLight["+ th +"]";
            glUniform3f(glGetUniformLocation(shaderProgram.Program, string(target+".position").c_str()), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
            glUniform3f(glGetUniformLocation(shaderProgram.Program, string(target+".ambient").c_str()), 0.0, 0.0, 0.0);
            glUniform3f(glGetUniformLocation(shaderProgram.Program, string(target+".diffuse").c_str()), 0.2, 0.2, 0.2);
            glUniform3f(glGetUniformLocation(shaderProgram.Program, string(target+".specular").c_str()), 1.0, 1.0, 1.0);
            glUniform1f(glGetUniformLocation(shaderProgram.Program, string(target+".constant").c_str()), 1.0);
            glUniform1f(glGetUniformLocation(shaderProgram.Program, string(target+".linear").c_str()), 0.27);
            glUniform1f(glGetUniformLocation(shaderProgram.Program, string(target+".quadratic").c_str()), 0.028);
        }
        // 配置手电筒参数
        glUniform3f(glGetUniformLocation(shaderProgram.Program, "spotLight.position"), camer.camerPos.x, camer.camerPos.y, camer.camerPos.z);
        glUniform3f(glGetUniformLocation(shaderProgram.Program, "spotLight.direction"), camer.camerFront.x, camer.camerFront.y, camer.camerFront.z);
        glUniform3f(glGetUniformLocation(shaderProgram.Program, "spotLight.ambient"), 0.1, 0.1, 0.1);
        glUniform3f(glGetUniformLocation(shaderProgram.Program, "spotLight.diffuse"), 0.2, 0.2, 0.2);
        glUniform3f(glGetUniformLocation(shaderProgram.Program, "spotLight.specular"), 1.0, 1.0, 1.0);
        glUniform1f(glGetUniformLocation(shaderProgram.Program, "spotLight.cutOff"), cos(glm::radians(12.5)));
        glUniform1f(glGetUniformLocation(shaderProgram.Program, "spotLight.outerCutOff"), cos(glm::radians(17.5)));

        // 变换光源
        /*
        glm::vec3 lightColor2(fabs(sin(glfwGetTime()*0.2)), fabs(sin(glfwGetTime()*0.7)), fabs(sin(glfwGetTime()*1.3)));
        glm::vec3 lAmbient = lightColor2*glm::vec3(0.1);
        glm::vec3 lDiffuse = lightColor2*glm::vec3(0.5);
        glm::vec3 lSpecular = lightColor2*glm::vec3(1.0);
        glUniform3f(lightAmbientLoc, lAmbient.x, lAmbient.y, lAmbient.z);
        glUniform3f(lightDiffuseLoc, lDiffuse.x, lDiffuse.y, lDiffuse.z);
        glUniform3f(lightSpecularLoc, lSpecular.x, lSpecular.y, lSpecular.z);
        */
        // 绑定 - 画图 - 解绑
        for(int i=0;i<10;++i)
        {
            // 位移
            glm::mat4 model = glm::translate(uniM4, cubePosition[i]);
            // 旋转
            GLfloat angle = 20.0*i;
            model = glm::rotate(model, angle, glm::vec3(1.0, 0.3, 0.5));

            glUniformMatrix4fv(glGetUniformLocation(shaderProgram.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        // 电灯着色起使用
        lightShader.Use();
        // 获取位置 & 配置位置参数
        for(int i=0;i<4;++i)
        {
            glUniform3f(glGetUniformLocation(lightShader.Program, "lightColor"), 1.0, 1.0, 1.0);
            // -配置摄像机参数
            glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glm::mat4 lightModel = glm::translate(uniM4, pointLightPositions[i]);
            lightModel = glm::scale(lightModel, glm::vec3(0.2));
            glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
            // 绑定 - 画图 - 解绑
            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }



        // 交换缓存，增强视觉效果
        glfwSwapBuffers(window);
        // 注册按键回调函数，根据用户输入去做相应的处理
        glfwSetKeyCallback(window, key_callback);
        // 注册鼠标回调函数，根据用户的鼠标移动做出相应的处理
        glfwSetCursorPosCallback(window, mouse_callback);
        // 注册鼠标滚轮回调函数，根据用户滚轮做出缩放处理
        glfwSetScrollCallback(window, scroll_callback);
    }
    // 结束后正确释放之前分配的所有资源
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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

    // 设置鼠标隐藏并且不能移除窗口
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return true;
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
// 回调函数（操作），根据按键去操作窗口
void key_callback(GLFWwindow *window, GLint key,  GLint scancode, GLint action, GLint mode)
{
    if(action == GLFW_PRESS)
        keyF[key] = GL_TRUE;
    else if(action == GLFW_RELEASE)
        keyF[key] = GL_FALSE;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

// 回调函数，根据鼠标位置跟新目标坐标
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    camer.doCursorMovement(xpos, ypos);
}

// 回调函数，根据鼠标滚轮做出缩放操作
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset)
{
    camer.doScrollMovement(xOffset, yOffset);
}

// 摄像机移动
void doCamerMovement()
{
    GLfloat curTime = (GLfloat)glfwGetTime();
    if(keyF[GLFW_KEY_W] == GLFW_TRUE)  // 摄像机前移（不是上）
    {
        camer.doCamerMovement(FORWARD, curTime);
    }
    if(keyF[GLFW_KEY_S] == GLFW_TRUE)  // 摄像机后移（不是下）
    {
        camer.doCamerMovement(BACKWARD, curTime);
    }
    if(keyF[GLFW_KEY_A] == GLFW_TRUE)  // 摄像机左移
    {
        camer.doCamerMovement(LEFT, curTime);
    }
    if(keyF[GLFW_KEY_D] == GLFW_TRUE)  // 摄像机右移
    {
        camer.doCamerMovement(RIGHT, curTime);
    }
    if(keyF[GLFW_KEY_SPACE] == GLFW_TRUE)
    {
        camer.doCamerMovement(UP, curTime);
    }
    if(keyF[GLFW_KEY_LEFT_SHIFT] == GLFW_TRUE)
    {
        camer.doCamerMovement(DOWN, curTime);
    }
}



