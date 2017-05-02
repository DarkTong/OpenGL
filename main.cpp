#include <iostream>
#include <cmath>
#include <string>
#include <map>

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
#include "Model.h"
//using namespace std;

// Window dimensions
const GLuint WIDTH = 1200, HEIGHT = 800;

// 颜色参数
//double red=0.2, green=0.3, bule=0.5;
double red=0.1, green=0.1, bule=0.1;

// 按键记录
GLboolean keyF[1024]= {0};

// 单位矩阵
glm::mat4 uniM4;

// 灯光
glm::vec3 lightColor(1.0, 1.0, 1.0);

// 缓冲对象
GLuint blockVAO, blockVBO, planeVAO, planeVBO, grassVBO, grassVAO;
// 纹理对象
GLuint blockTexture, planeTexture, grassTexture;

// cube
GLfloat blockVertices[] = {
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left
};
int blockPointNum = 36*5;

// 地板坐标
GLfloat planeVertices[] = {
    // Positions          // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
    5.0f,  -0.51f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.51f,  5.0f,  0.0f, 0.0f,
    -5.0f, -0.51f, -5.0f,  0.0f, 2.0f,

    5.0f,  -0.51f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.51f, -5.0f,  0.0f, 2.0f,
    5.0f,  -0.51f, -5.0f,  2.0f, 2.0f
};
int planePointNum = 6*5;

// 草坐标
GLfloat grassVertices[] = {
    // position         // texture
    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};
int grassPointNum = 6*5;

// block位置
glm::vec3 blockPosition[] = {
    glm::vec3(-1.0, 0.0, -1.0),
    glm::vec3(2.0, 0.0, 0.0),
};
GLuint blockNum = 2;

// plane位置
glm::vec3 planePosition[] = {
    glm::vec3(0.0, 0.0, 0.0),
};
GLuint planeNum = 1;

// 草位置
vector<glm::vec3> grassPosition;
GLuint grassNum = 4;

// 光源位置
glm::vec3 pointLightPositions[] =
{
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

/* 创建摄影机对象 */
Camer camer((float)WIDTH, (float)HEIGHT,
        glm::vec3(0.0, 0.0, 1.0));

// Function prototypes
void init();
int buildWindow(GLFWwindow *, const int &width, const int &height);
void changeRGB();
void doCamerMovement();
void key_callback(GLFWwindow *, GLint,  GLint, GLint, GLint);
void mouse_callback(GLFWwindow *, double, double);
void scroll_callback(GLFWwindow *, double, double);
void BindVBOAndVAO(GLuint &VBO, GLuint &VAO, GLfloat* vertices, int pointNum);
void BindTexture(GLuint &texture, const char* path);
//void DrawContainers(int flag, Shader &shader, glm::mat4 projection, glm::mat4 view, glm::mat4 model);
void sortPosition(map<GLfloat, int> &hasSortPos, glm::vec3 camerPos, vector<glm::vec3> &mArray);

int main()
{
    /* 设置草坐标 */
    grassPosition.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
    grassPosition.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
    grassPosition.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
    grassPosition.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
    grassPosition.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));

    /* 初始化环境 */
    init();

    /* 新建第一个窗口 */
    // 获取新建的窗口指针
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if(buildWindow(window, WIDTH, HEIGHT) == -1)
        return -1;

    /* 1.创建顶点着色器和线段着色器 */
    Shader blockShader("./shader/vecS/block.vs", "./shader/fs/block.fs");
    Shader planeShader("./shader/vecS/plane.vs", "./shader/fs/plane.fs");
    Shader grassShader("./shader/vecS/grass.vs", "./shader/fs/grass.fs");
    Shader colorShader("./shader/vecS/lightColor.vs", "./shader/fs/lightColor.fs");

    /* 创建模块 */
    //Model nanosite("./model/nanosuit/nanosuit.obj");


    /* bind block */
    BindVBOAndVAO(blockVBO, blockVAO, blockVertices, blockPointNum);

    /* plane */
    BindVBOAndVAO(planeVBO, planeVAO, planeVertices, planePointNum);

    /* gress */
    BindVBOAndVAO(grassVBO, grassVAO, grassVertices, grassPointNum);

    /* blockTexture */
    BindTexture(blockTexture, "./texture/block.jpg");

    /* planeTexture */
    BindTexture(planeTexture, "./texture/plane.jpg");

    /* grassTexture */
    BindTexture(grassTexture, "./texture/blending_transparent_window.png");

    /* 画图（渲染）*/
    while(!glfwWindowShouldClose(window)) // 检查GLFW是否被要求退出
    {
        // 检查事件， 检查有没有触发什么时间（键盘输入和鼠标移动）
        glfwPollEvents(); //应该右使用像素缓冲的功能，由于调试的时候到这一步，屏幕就会变亮
        // 摄影机移动
        doCamerMovement();

        // 渲染指令， 创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了。（背景色）
        glClearColor(red, green, bule, 0.01f);
        // 清除颜色缓存，清除深度缓存
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //changeRGB();

        // 模块空间
        glm::mat4 model;
        // 构造观察空间
        glm::mat4 view = camer.getViewMartix();
        // 构造裁剪空间
        glm::mat4 projection = glm::perspective(camer.camerFov, (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.1f, 300.0f);

        /* normalblock */
        /* 开启面剔除模式 */
        glEnable(GL_CULL_FACE);
        // 剔除方块的前面
        glCullFace(GL_FRONT);
        // 剔除前面顺时针渲染的面
        // GL_CCW 表示逆时针，GL_CW 表示顺时针
        // glFrontFace(); 表示把GLCCW或GL_CW当成是正面
        glFrontFace(GL_CW);
        blockShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(blockShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(blockShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        // 纹理
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(blockShader.Program, "texture_diffuse1"), 0);
        glBindTexture(GL_TEXTURE_2D, blockTexture);
        for(GLuint i = 0; i < blockNum; ++i)
        {
            model = glm::translate(uniM4, blockPosition[i]);
            glUniformMatrix4fv(glGetUniformLocation(blockShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(blockVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        /* 关闭面剔除模式 */
        glDisable(GL_CULL_FACE);

        /* plane */
        planeShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(planeShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(planeShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        // 纹理
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(planeShader.Program, "texture_diffuse1"), 1);
        glBindTexture(GL_TEXTURE_2D, planeTexture);
        for(GLuint i = 0; i < planeNum; ++i)
        {
            model = glm::translate(uniM4, grassPosition[i]);
            glUniformMatrix4fv(glGetUniformLocation(planeShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(planeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        /* grass */
        map<GLfloat, int> hasSortPos;
        sortPosition(hasSortPos, camer.camerPos, grassPosition);
        grassShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(grassShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(grassShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        // 纹理
        glActiveTexture(GL_TEXTURE2);
        glUniform1i(glGetUniformLocation(grassShader.Program, "texture_diffuse1"), 2);
        glBindTexture(GL_TEXTURE_2D, grassTexture);
        //for(GLuint i = 0; i < grassNum; ++i)
        for(map<GLfloat, int>::reverse_iterator it = hasSortPos.rbegin(); it != hasSortPos.rend(); ++it)
        {
            //model = glm::translate(uniM4, grassPosition[i]);
            model = glm::translate(uniM4, grassPosition[it->second]);
            glUniformMatrix4fv(glGetUniformLocation(grassShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(grassVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        // 交换缓存，增强视觉效果
        glfwSwapBuffers(window);

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

    // 注册按键回调函数，根据用户输入去做相应的处理
    glfwSetKeyCallback(window, key_callback);
    // 注册鼠标回调函数，根据用户的鼠标移动做出相应的处理
    glfwSetCursorPosCallback(window, mouse_callback);
    // 注册鼠标滚轮回调函数，根据用户滚轮做出缩放处理
    glfwSetScrollCallback(window, scroll_callback);

    // 设置鼠标隐藏并且不能移除窗口
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* 开启深度测试 */
    // 意思：检测深度，那么渲染时便会根据深度选择如何渲染
    glEnable(GL_DEPTH_TEST);

    /* 开启模式检测 */
    //glEnable(GL_STENCIL_TEST);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    /* 开启混合模式 */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

// 绑定VAO， VBO
void BindVBOAndVAO(GLuint &VBO, GLuint &VAO, GLfloat* vertices, int pointNum)
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBufferData(GL_ARRAY_BUFFER, pointNum*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

// 绑定纹理
void BindTexture(GLuint &texture, const char* path)
{
    unsigned char* image;
    int width, height;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    // 设置纹理属性
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
}

// 按与摄像机的距离进行排序
void sortPosition(map<GLfloat, int> &hasSortPos, glm::vec3 camerPos, vector<glm::vec3> &mArray)
{
    for(GLuint i = 0;i < mArray.size(); ++i)
    {
        GLfloat distance = glm::length(camerPos - mArray[i]);
        hasSortPos[distance] = i;
    }
}
