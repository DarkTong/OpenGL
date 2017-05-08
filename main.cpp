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
const GLuint WIDTH = 800, HEIGHT = 600;

// 颜色参数
double red=1.0, green=1.0, blue=1.0;
//double red=0.1, green=0.1, blue=0.1;

// 按键记录
GLboolean keyF[1024]= {0};

// 单位矩阵
glm::mat4 uniM4;

// 灯光
glm::vec3 lightColor(1.0, 1.0, 1.0);

// 缓冲对象
GLuint quadVAO, cubeVAO, planeVAO;
// 纹理对象
GLuint quadTex, cubeTex;
// uniform对象
GLuint uboMatrices;
// 实例化数组缓冲对象

// light position
glm::vec3 lightPos = glm::vec3(-2.0, 4.0, -1.0);

struct DrawPara
{
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3* position;
    GLuint posNum, poiNum, shaderProgramId, textureID, VAOID;
    DrawPara(glm::mat4 Pro, glm::mat4 View, glm::vec3* Pos,
             GLuint PosN, GLuint PoiN, GLuint ShaderProgramID, GLuint TextureID, GLuint VaoID)
             {
                 projection = Pro;
                 view = View;
                 position = Pos;
                 posNum = PosN;
                 poiNum = PoiN;
                 shaderProgramId = ShaderProgramID;
                 textureID = TextureID;
                 VAOID = VaoID;
             }
};

/* 创建摄影机对象 */
Camer camer((float)WIDTH, (float)HEIGHT,
        glm::vec3(0.0, 0.0, 2.0));

// Function prototypes
void init();
int buildWindow(GLFWwindow *, const int &width, const int &height);
void changeRGB();
void doCamerMovement();
void key_callback(GLFWwindow *, GLint,  GLint, GLint, GLint);
void mouse_callback(GLFWwindow *, double, double);
void scroll_callback(GLFWwindow *, double, double);
void BindVBOAndVAO(GLuint &VBO, GLuint &VAO, GLfloat* vertices, int pointNum);
GLuint BindTexture(const char* path, int alpha = 0);
GLuint BindCubeTexture(const string path);
GLuint generateAttachmentTexture(GLboolean depth = GL_FALSE, GLboolean stencil = GL_FALSE);
void sortPosition(map<GLfloat, int> &hasSortPos, glm::vec3 camerPos, vector<glm::vec3> &mArray);
void DrawTriangle(const DrawPara data);
// 渲染
void RenderShader(Shader shader);
void RenderQuad();
void RenderCube();
void RenderPlane();

/* temp */
void buildModelArray(glm::mat4*& modelArray, int num);

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
    Shader shader("./shader/vecS/plane.vs", "./shader/fs/blinn_phong.frag", "");
    Shader shadowShader("./shader/vecS/shadowCube.vs", "./shader/fs/shadowCube.frag", "./shader/geom/shadowCube.geom");
    Shader debugDepthMapShader("./shader/vecS/debugDepthMap.vs", "./shader/fs/debugDepthMap.frag", "./shader/geom/Triangle.geom");

    /* texture */
    // 生成深度贴图 - cube
    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
    // 纹理附件,深度值用浮点值表示-GL_FLOAT - 为每个面申请空间
    for(GLuint i=0;i<6;++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    /* fragment */
    GLuint depthMapVBO;
    glGenFramebuffers(1, &depthMapVBO);
    // 作为帧缓冲的深度缓冲 - cube
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapVBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /* parameter */
    // shadowShader
    GLfloat near_plane = 0.1, far_plane = 100.0, aspect = (GLfloat)SHADOW_WIDTH/(GLfloat)SHADOW_HEIGHT;
    glm::mat4 shadowProj = glm::perspective((GLfloat)90.0, aspect, near_plane, far_plane);
    vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj *
                glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0,1.0,0.0)));
    shadowTransforms.push_back(shadowProj *
                glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,1.0,0.0)));
    shadowTransforms.push_back(shadowProj *
                glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0,0.0,1.0)));
    shadowTransforms.push_back(shadowProj *
                glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0,0.0,-1.0)));
    shadowTransforms.push_back(shadowProj *
                glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0,1.0,0.0)));
    shadowTransforms.push_back(shadowProj *
                glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0,1.0,0.0)));

    /* uniform内存 */
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // 用于指定哪块uniformBlock绑定哪段内存，并且为uniformBlock绑定标号(0)
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2*sizeof(glm::mat4));

    // 绑定Shader 中的 uniform Block
    glUniformBlockBinding(shader.Program,
                              glGetUniformBlockIndex(shader.Program, "Matrices"), 0);
    glUniformBlockBinding(debugDepthMapShader.Program,
                              glGetUniformBlockIndex(debugDepthMapShader.Program, "Matrices"), 0);

    /* 画图（渲染）*/
    while(!glfwWindowShouldClose(window)) // 检查GLFW是否被要求退出
    {
        // 检查事件， 检查有没有触发什么时间（键盘输入和鼠标移动）
        glfwPollEvents(); //应该右使用像素缓冲的功能，由于调试的时候到这一步，屏幕就会变亮
        // 摄影机移动
        doCamerMovement();

        // 1.首先渲染深度贴图
//        glEnable(GL_CULL_FACE);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapVBO);
        // 清空帧缓冲数据
        //glClear(GL_DEPTH_BITS);  // --> 写错了
        glClear(GL_DEPTH_BUFFER_BIT);
        // --ConfigureShaderAndMatrices
        // ----由于是定点光源，所以使用透视投影
        shadowShader.Use();
        for(int i=0;i<6;++i)
        {
            GLuint loc = glGetUniformLocation(shadowShader.Program, ("shadowMatrices[" + std::__cxx11::to_string(i) + "]").c_str());
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(shadowTransforms[i]));
        }
        glUniform3f(glGetUniformLocation(shadowShader.Program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform1f(glGetUniformLocation(shadowShader.Program, "far_plane"), far_plane);
        glUniformMatrix4fv(glGetUniformLocation(shadowShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4()));
        // --RenderScene
    //    glCullFace(GL_FRONT);
        RenderShader(shadowShader);
  //      glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        glDisable(GL_CULL_FACE);

        // 2.正常渲染
        glViewport(0, 0, WIDTH, HEIGHT);
        // 清空数据
        glEnable(GL_DEPTH_TEST);
        glClearColor(red, green, blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // --ConfigureShaderAndMatricecs
        glm::mat4 view = camer.getViewMartix();
        glm::mat4 projection = glm::perspective(camer.camerFov, (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.1f, 100.0f);
        // 写入uniformBuffer
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        shader.Use();
        if(quadTex == 0)
            quadTex = BindTexture("./texture/plane2.jpg", 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, quadTex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
        glUniform1i(glGetUniformLocation(shader.Program, "texture_diffuse1"), 0);
        glUniform1i(glGetUniformLocation(shader.Program, "depthMap"), 1);
        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4()));
        glUniform3f(glGetUniformLocation(shader.Program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(shader.Program, "camerPos"), camer.camerPos.x, camer.camerPos.y, camer.camerPos.z);
        glUniform1f(glGetUniformLocation(shader.Program, "far_plane"), far_plane);
        // --RenderScene
        RenderShader(shader);

        // 3.Render depth map
        debugDepthMapShader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
        //RenderQuad();



/*
        // 开启深度测试
        glEnable(GL_DEPTH_TEST);
        // 渲染指令， 创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了。（背景色）
        glClearColor(red, green, blue, 1.0f);
        // 清除颜色缓存，清除深度缓存
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 模块空间
        glm::mat4 model;
        // 构造观察空间
        glm::mat4 view = camer.getViewMartix();
        // 构造裁剪空间
        glm::mat4 projection = glm::perspective(camer.camerFov, (GLfloat)WIDTH/(GLfloat)HEIGHT, 0.1f, 600.0f);
        // 写入uniformBuffer
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        // plane
        planeShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(planeShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3f(glGetUniformLocation(planeShader.Program, "lightPos"), 1.0, 2.0, 1.5);
        glUniform3f(glGetUniformLocation(planeShader.Program, "camerPos"), camer.camerPos.x, camer.camerPos.y, camer.camerPos.z);
        glBindTexture(GL_TEXTURE_2D, planeTexture);
        glUniform1f(glGetUniformLocation(planeShader.Program, "ambient"), 0.2);
        glBindVertexArray(planeVAO);
        //glEnable(GL_FRAMEBUFFER_SRGB);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDisable(GL_FRAMEBUFFER_SRGB);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
*/
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
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* 开启深度测试 */
    // 意思：检测深度，那么渲染时便会根据深度选择如何渲染
    //glEnable(GL_DEPTH_TEST);

    /* 开启模式检测 */
    //glEnable(GL_STENCIL_TEST);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    /* 开启混合模式 */
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

// 渲染操作函数
void changeRGB()
{
    red += 0.0001;
    green += 0.0002;
    blue += 0.00001;
    if(red > 0.7) red -= 0.6;
    if(green > 0.6) green -= 0.3;
    if(blue > 0.9) blue -= 0.6;
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
    if(keyF[GLFW_KEY_C] == GLFW_TRUE)
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
GLuint BindTexture(const char* path, int alpha)
{
    unsigned char* image;
    int width, height;
    GLuint texture;
    GLenum type = alpha ? GL_RGB : GL_RGBA;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGB : SOIL_LOAD_RGBA);
    // 申请内存空间，并写入image数据
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, image);
    // 多级纹理
    glGenerateMipmap(GL_TEXTURE_2D);
    // 设置纹理属性
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_MIRRORED_REPEAT : GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_MIRRORED_REPEAT: GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return texture;
}

// 立方体纹理加载
GLuint BindCubeTexture(const string path)
{
    int width, height;
    unsigned char* image = NULL;
    vector<string> faces;

    /* 获取立方体各面路径 */
    faces.push_back(path + "right.jpg");
    faces.push_back(path + "left.jpg");
    faces.push_back(path + "top.jpg");
    faces.push_back(path + "bottom.jpg");
    faces.push_back(path + "back.jpg");
    faces.push_back(path + "front.jpg");

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0;i < faces.size(); ++i)
    {
        image = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        if(image == NULL)
            cout << "ERROR:: path:" << faces[i] << "\n" << endl;
        // 为各个面写入纹理
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    }
    //glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

// 纹理附件
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
    GLuint texture;
    GLenum attachmentType;
    // 设置type
    if(!depth && ! stencil)
        attachmentType = GL_RGB;
    else if(depth && !stencil)
        attachmentType = GL_DEPTH_COMPONENT;
    else if(!depth && stencil)
        attachmentType = GL_STENCIL_INDEX;
    // 常规操作
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    if(!depth && !stencil)
        glTexImage2D(GL_TEXTURE_2D, 0, attachmentType, WIDTH, HEIGHT, 0, attachmentType, GL_UNSIGNED_BYTE, NULL);
    else // 特殊格式
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
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

// 三角形渲染
void DrawTriangle(const DrawPara data)
{
    glUniformMatrix4fv(glGetUniformLocation(data.shaderProgramId, "projection"), 1, GL_FALSE, glm::value_ptr(data.projection));
    glUniformMatrix4fv(glGetUniformLocation(data.shaderProgramId, "view"), 1, GL_FALSE, glm::value_ptr(data.view));
    // 纹理
    glBindTexture(GL_TEXTURE_2D, data.textureID);
    for(GLuint i = 0; i < data.posNum; ++i)
    {
        glm::mat4 model = glm::mat4(0.0);
        model = glm::translate(uniM4, data.position[i]);
        glUniformMatrix4fv(glGetUniformLocation(data.shaderProgramId, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(data.VAOID);
        glDrawArrays(GL_TRIANGLES, 0, data.poiNum);
        glBindVertexArray(0);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

// 获取位移数据
void GetOffsetPosition(glm::vec2* offsetPosition)
{
    int cnt = 0;
    GLfloat offset = 0.2;
    GLfloat xPos, yPos;
    yPos = 0.9;
    for(int i = 0; i < 10; ++i)
    {
        xPos = -0.9;
        for(int j=0;j < 10; ++j)
        {
            offsetPosition[cnt++] = glm::vec2(xPos, yPos);
            xPos += offset;
        }
        yPos -= offset;
    }
}

// 获取rock model数据
void buildModelArray(glm::mat4*& modelArray, int num)
{
    modelArray = new glm::mat4[num];
    srand(glfwGetTime());
    GLfloat radius = 150.0;
    GLfloat offset = 50;

    for(int i = 0; i < num; ++i)
    {
        glm::mat4 model;
        // 1. Translation: display along circle with 'radius'  in range [-offset, offset]
        GLfloat angle = i*1.0 / num * 360.0;
        GLfloat displacement = (rand() % (GLint)(2*offset*100)) / 100.0 - offset; //保证范围在[-offset, offset]内
        GLfloat x = radius * sin(angle) + displacement;
        displacement = (rand() % (GLint)(2*offset*100)) / 100.0 - offset;
        GLfloat y = displacement * 0.5;
        displacement = (rand() % (GLint)(2*offset*100)) / 100.0 - offset;
        GLfloat z = radius * cos(angle) + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));
        // 2.Scale: Scale between 0.05, 0.25f;
        GLfloat scale = (rand()%20 + 5) / 100.0;
        model = glm::scale(model, glm::vec3(scale));
        // 3.Rotation: add random rotation a (semi)randomly picked axis vector
        GLfloat rotAngle = (rand()%360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.2, 0.3, 0.4));
        modelArray[i] = model;
    }
}

// 渲染现场
void RenderShader(Shader shader)
{
    // plane
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0));
    model = glm::scale(model, glm::vec3(20.0));
    glUniform1i(glGetUniformLocation(shader.Program, "reverse_normals"), 1);
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    RenderCube();
    glUniform1i(glGetUniformLocation(shader.Program, "reverse_normals"), 0);
    //RenderPlane();
    // cube
    int cubeNum = 3;
    glm::vec3 cubePosition[] = {
        glm::vec3(0.0, 1.5, 0.0),
        glm::vec3(2.0, -0.501, 1.0),
        glm::vec3(-1.0, 0.0, 2.0),
    };
    for(int i = 0; i < cubeNum; ++i)
    {
        model = glm::mat4();
        model = glm::translate(model, cubePosition[i]);
        if(i == cubeNum - 1)
            model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 2.0, 3.0)));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        RenderCube();
    }
}

// 渲染地板
void RenderPlane()
{
    if( planeVAO == 0)
    {
        GLfloat planeVertices[] = {
            // Positions            // Normals         // Texture Coords
            25.0f, -1.0f, 25.0f,   0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
            -25.0f, -1.0f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
            -25.0f, -1.0f, 25.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

            25.0f, -1.0f, 25.0f,   0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
            25.0f, -1.0f, -25.0f,  0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
            -25.0f, -1.0f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f
        };
        GLuint planeVBO;
        glGenBuffers(1, &planeVBO);
        glGenVertexArrays(1, &planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBindVertexArray(planeVAO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
        glBindVertexArray(0);
    }
//    if(quadTex == 0)
//        quadTex = BindTexture("./texture/plane2.jpg", 1);
//    glBindTexture(GL_TEXTURE_2D, quadTex);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
//    glBindTexture(GL_TEXTURE_2D, 0);
}

// 渲染立方体
void RenderCube()
{
    if( cubeVAO == 0)
    {
        GLfloat cubeVertices[] = {
            // Back face
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
            // Front face
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
            // Left face
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            // Right face
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
            // Bottom face
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            // Top face
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left
        };
        GLuint cubeVBO;
        glGenBuffers(1, &cubeVBO);
        glGenVertexArrays(1, &cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindVertexArray(cubeVAO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(0));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
        glBindVertexArray(0);
    }
//    if(cubeTex == 0)
//        cubeTex = BindTexture("./texture/container2.png", 1);
//    glBindTexture(GL_TEXTURE_2D, cubeTex);
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
//    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderQuad()
{
    if( quadVAO == 0)
    {
        GLfloat quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
             1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
             1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        };
        GLuint quadVBO;
        glGenBuffers(1, &quadVBO);
        glGenVertexArrays(1, &quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBindVertexArray(quadVAO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
        glBindVertexArray(0);
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}


