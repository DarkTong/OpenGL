#include <iostream>
#include <cstdio>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// 颜色参数
double red=0.1, green=0.3, bule=0.3;

// 定点数量
GLint numberOfVertex=12;

GLfloat vertices1[]=
{
    0.5f, 0.0f, 0.0f,   //中右
    // buttom
    -0.5f, -0.5f, 0.0f, //左下
    0.0f, -0.5f, 0.0f
};

// 三角形定点坐标
GLfloat vertices[]=
{
    // top
    -0.5, 0.5f, 0.0f,   //左上
    0.0f, 0.5f, 0.0f,   //中上
    0.5f, 0.5f, 0.0f,   //右上
    // middle
    -0.5f, 0.0f, 0.0f,  //中左
    0.0f, 0.0f, 0.0f,   //中中
    0.5f, 0.0f, 0.0f,   //中右
    // buttom
    -0.5f, -0.5f, 0.0f, //左下
    0.0f, -0.5f, 0.0f,  //中下
    0.5f, -0.5f, 0.0f   //右下
};

GLuint indices[]=
{
    0, 1, 3,    //第一个三角形
    1, 2, 5,    //第二个三角形
    3, 6, 7,
    5, 7, 8
};

// 定点着色器源码
const GLchar* vertexShaderSource="#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main(){\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\n\0";
// 片段着色器源码
const GLchar* fragmentShaderSource="#version 330 core\n"
    "out vec4 color;\n"
    "void main(){\n"
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
const GLchar* fragmentShaderSource1="#version 330 core\n"
    "out vec4 color;\n"
    "void main(){\n"
    "color = vec4(0.8f, 0.8f, 0.8f, 1.0f);\n"
    "}\n\0";

// Function prototypes
void key_callback(GLFWwindow *, GLint,  GLint, GLint, GLint);
void changeRGB();
void shaderCompileInfo(GLuint&);
void shaderLinkInfo(GLuint&);
int main()
{

    /* test the number of Vertex Attribute */
    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maxinum nr of vertex attributes supported: " << nrAttributes << std::endl;

    /*初始化环境
    */
    // 初始化GLFW
    glfwInit();
    // 设置版本信息
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 利用核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // 窗口大小不改变
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);

    /*新建第一个窗口
    */

    // 获取新建的窗口指针
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
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
    // 设置窗口大小参数
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // 左上角坐标和右下角坐标
    glViewport(0, 0, width, height);

    /* 1.创建顶点着色器和线段着色器 */
    // 编译定点着色器
    GLuint vertexShader, vertexShader1;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // -2
    vertexShader1 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader1, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader1);
    // 检测是否编译错误
    shaderCompileInfo(vertexShader);
    shaderCompileInfo(vertexShader1);

    // 编译片段着色器
    GLuint fragmentShader, fragmentShader1;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // -2
    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShader1);
    // 检测是否编译错误
    shaderCompileInfo(fragmentShader);
    shaderCompileInfo(fragmentShader1);

    /* 2.构建着色器程序 */
    // 链接着色器的程序
    GLuint shaderProgram, shaderProgram1;
    shaderProgram = glCreateProgram();
    // 添加着色器（按顺序）(由于原理是利用前一个输出作为后一个的输入）；
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // -2

    shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader1);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);


    /* 3.创建顶点缓存对象,顶点数组对象 */
    GLuint VBO, VBO1; //Vertex Buffer Objects
    GLuint VAO, VAO1; //Vertex Array Object
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // 3.4解绑VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //申请
    glGenBuffers(1, &VBO1);
    glGenVertexArrays(1, &VAO1);
    //绑定
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBindVertexArray(VAO1);
    //写进BVBO1
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices1, vertices1, GL_STATIC_DRAW);
    //设置定点指针属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //解绑
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* 画图（渲染） */
    // 为了使画图程序不会在执行完后立刻退出
        // 检查事件， 检查有没有触发什么时间（键盘输入和鼠标移动）
    while(!glfwWindowShouldClose(window)) // 检查GLFW是否被要求退出
    {
        glfwPollEvents();

        // 渲染指令， 创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了。
        glClearColor(red, green, bule, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        changeRGB();

        // 渲染指令，绘制物体
        glUseProgram(shaderProgram);    //加载着色器程序

        /* 一个VBO、VAO
        // 绑定
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // 画图
        //someOpenGLFunctionDrawsOurTriangle();
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawArrays(GL_TRIANGLES, 3, 3);
        glDrawElements(GL_TRIANGLES, numberOfVertex, GL_UNSIGNED_INT, 0);

        // 解绑
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBindVertexArray(0);
        */
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 1, 3);
        glBindVertexArray(0);
        glUseProgram(shaderProgram1);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
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

// 回调函数（操作），根据按键去操作窗口
void key_callback(GLFWwindow *window, GLint key,  GLint scancode, GLint action, GLint mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

// 渲染操作函数
void changeRGB()
{
    red += 0.001;
    green += 0.002;
    bule += 0.0001;
    if(red > 0.7) red -= 0.6;
    if(green > 0.6) green -= 0.3;
    if(bule > 0.9) bule -= 0.6;
}

// 检测着色器是否编译成功
void shaderCompileInfo(GLuint& shader)
{
    // 检查着色器是否编译错误
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    // 若变异错误则输出
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR:COMPILE FAILED\n" << infoLog << std::endl;
    }
}

// 检测链接器是否正确链接
void shaderLinkInfo(GLuint& shaderProgram)
{
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR:LINK FAILED\n" << infoLog << std::endl;
    }
}
