#include "Camer.h"

// 常数
const GLfloat Camer::WIDTH=800.0;
const GLfloat Camer::HEIGHT=600.0;
const GLfloat Camer::PITCH=0.0;
const GLfloat Camer::YAW=-90.0;
const GLfloat Camer::SENSITIVITY=0.05;
//const GLfloat Camer::SPEED=0.0001;
const GLfloat Camer::SPEED=0.003;
const GLfloat Camer::FOV=45.0;
const glm::vec3 Camer::WUP = glm::vec3(0.0, 1.0, 0.0);
const glm::vec3 Camer::FRONT = glm::vec3(0.0, 0.0, -1.0);
const glm::vec3 Camer::POS = glm::vec3(0.0, 0.0, 1.0);

Camer::Camer(GLfloat width, GLfloat height, glm::vec3 pos,
             glm::vec3 worldUp, GLfloat pitch, GLfloat yaw)
{
    // 初始化按键信息
    this->camerPos = pos;
    this->worldUp =worldUp;
    // 摄影机角度参数
    this->pitch = pitch;
    this->yaw = yaw;
    // 摄影机视野
    this->camerFov = Camer::FOV;
    // 帧时间
    this->currentFrame = this->lastFrame = 0.0f;
    // 鼠标位置
    this->mouseLastPosX = width/2;
    this->mouseLastPosY = height/2;
    this->mouseCurPosX = mouseCurPosY = 0;
    // 鼠标灵敏度
    this->sensitivity = Camer::SENSITIVITY;
    // 鼠标移动常熟
    this->camerSpeedC = Camer::SPEED;
    // 鼠标还没进入
    this->first = GL_TRUE;

    this->updateCamer();
}

Camer::~Camer()
{
    //dtor
}

// 更新摄像机参数
void Camer::updateCamer()
{
    glm::vec3 ffront;
    ffront.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
    ffront.y = sin(glm::radians(this->pitch));
    ffront.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));

    // 更新摄影机向量
    this->camerFront = glm::normalize(ffront);
    this->camerRight = glm::normalize(glm::cross(this->camerFront, this->worldUp));
    this->camerUp = glm::normalize(glm::cross(this->camerRight, this->camerFront));
    //this->camerRight = glm::normalize(glm::cross(this->worldUp, this->camerFront));
    //this->camerUp = glm::normalize(glm::cross(this->camerFront, this->camerRight));

    // FPS
    this->forFPS();
}

// 摄影机移动
void Camer::doCamerMovement(Camer_Movement direction, GLfloat curTime)
{
    this->currentFrame = curTime;
    GLfloat deltTime = (currentFrame - lastFrame);
    GLfloat camerSpeed = this->camerSpeedC;// * deltTime;
    this->lastFrame = currentFrame;
    // 摄像机前移
    glm::vec3 tmp;
    if(direction == FORWARD)
    {
        this->camerPos += camerSpeed * this->camerFront;
    }
    else if(direction == BACKWARD)
    {
        this->camerPos -= camerSpeed * this->camerFront;
    }
    else if(direction == LEFT)
    {
        this->camerPos -=  camerSpeed * this->camerRight;
    }
    else if(direction == RIGHT)
    {
        this->camerPos +=  camerSpeed * this->camerRight;
    }
    else if(direction == UP)
    {
        this->camerPos += camerSpeed * this->camerUp;
    }
    else if(direction == DOWN)
    {
        this->camerPos -= camerSpeed * this->camerUp;
    }
    this->updateCamer();
}

// 鼠标移动
void Camer::doCursorMovement(GLfloat xpos, GLfloat ypos)
{
    if(this->first)
    {
        mouseLastPosX = xpos;
        mouseLastPosY = ypos;
        first = GL_FALSE;
    }

    this->mouseCurPosX = xpos;
    this->mouseCurPosY = ypos;

    // 鼠标的位移
    GLfloat xOffset, yOffset;
    xOffset = (this->mouseCurPosX - this->mouseLastPosX) * this->sensitivity;
    yOffset = (this->mouseCurPosY - this->mouseLastPosY) * this->sensitivity;
    this->mouseLastPosX = this->mouseCurPosX;
    this->mouseLastPosY = this->mouseCurPosY;

    this->yaw += xOffset;
    this->pitch -= yOffset;

    if(this->pitch >= 80.0)
        this->pitch = 80.0;
    if(this->pitch <= -80.0)
        this->pitch = -80.0;

    this->updateCamer();
}

// 滚轮移动
void Camer::doScrollMovement(GLfloat xOffset, GLfloat yOffset)
{
    // yOffset代表竖滚轮
    this->camerFov -= yOffset;
    if(this->camerFov > 80.0)
        this->camerFov = 80.0;
    else  if(this->camerFov < 10.0)
        this->camerFov = 10.0;
}
// 获取观察空间
glm::mat4 Camer::getViewMartix()
{
    return glm::lookAt(this->camerPos, this->camerPos + this->camerFront, this->worldUp);
}

