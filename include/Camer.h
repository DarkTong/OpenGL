#ifndef CAMER_H
#define CAMER_H

#include <cmath>
#include <cstring>

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

#include <Shader.h>

// 按键类型
enum Camer_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camer
{
public:
    static const GLfloat WIDTH;
    static const GLfloat HEIGHT;
    static const GLfloat PITCH;
    static const GLfloat YAW;
    static const GLfloat SENSITIVITY;
    static const GLfloat SPEED;
    static const GLfloat FOV;
    static const glm::vec3 WUP;
    static const glm::vec3 FRONT;
    static const glm::vec3 POS;

    // 摄影机位置参数
    glm::vec3 camerRight;
    glm::vec3 camerFront;
    glm::vec3 camerUp;
    glm::vec3 camerPos;
    glm::vec3 worldUp;
    // 摄影机角度参数（俯仰角、偏航角）
    GLfloat pitch;
    GLfloat yaw;
    // 鼠标灵敏度
    GLfloat sensitivity;
    // 摄影机移动速度常数
    GLfloat camerSpeedC;
    // 摄像机视野
    GLfloat camerFov;

    Camer(GLfloat width, GLfloat height, glm::vec3 pos=glm::vec3(0.0, 0.0, 1.0),
          glm::vec3 worldUp=glm::vec3(0.0, 1.0, 0.0), GLfloat pitch=PITCH, GLfloat yaw = YAW);
    virtual ~Camer();

    // 按键移动
    void doCamerMovement(Camer_Movement, GLfloat);
    // 鼠标移动
    void doCursorMovement(GLfloat, GLfloat);
    // 滚轮移动
    void doScrollMovement(GLfloat, GLfloat);
    // 获取观察空间
    glm::mat4 getViewMartix();

protected:

private:
    // 判断鼠标是否第一次进入
    GLboolean first;
    // 帧时间
    GLfloat currentFrame, lastFrame;
    // 鼠标位置
    GLfloat mouseCurPosX, mouseCurPosY, mouseLastPosX, mouseLastPosY;
    // 更新摄像机参数
    void updateCamer();
    // fps摄像机
    void forFPS()
    {
        //camerPos.y = POS.y;
    }
};



















#endif // CAMER_H
