#include "Mesh.h"
// 构造
Mesh::Mesh(vector<Vertex> vertices, vector<Texture> textures, vector<GLuint> indices)
{
    // 获取数据
    this->vertices = vertices;
    this->textures = textures;
    this->indices = indices;
    // 现在我们已经获得所需要的数据，所以开始设置VBO,VAO,EBO
    this->setupMesh();
}

// 复制构造
/*
Mesh::Mesh(const Mesh &mesh)
{
    this->vertices = mesh.vertices;
    this->textures = mesh.textures;
    this->indices = mesh.indices;

    this->VAO = mesh.VAO;
    this->VBO = mesh.VBO;
    this->EBO = mesh.EBO;
}
*/

// 析构
Mesh::~Mesh()
{

}

// 绑定VAO、VBO、BEO
void Mesh::setupMesh()
{
    // 申请对象
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    // 绑定
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // 写数据
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
                 &this->vertices[0], GL_STATIC_DRAW);
    // 写数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
                 &this->indices[0], GL_STATIC_DRAW);

    // 设定指针
    // -定点坐标
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);
    // -法向量
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    // -纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    //解绑
    glBindVertexArray(0);
}

//
void Mesh::Draw(Shader shader)
{
    // 纹理数量
    /* example
     * struct Material
     * {
     *     uniform sampler2D texture_diffuse1;
     *     uniform sampler2D texture_diffuse2;
     *     uniform sampler2D texture_specular1;
     * }
     * 下标从1开始
     */
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    // 绑定纹理单元
    for(GLuint i=0; i<this->textures.size(); ++i)
    {
        // 选择活跃纹理单元
        glActiveTexture(GL_TEXTURE0+i);
        string idx;
        string name = this->textures[i].type;
        if(name == "texture_diffuse")
            idx = std::__cxx11::to_string(diffuseNr++);
        else if(name == "texture_specular")
            idx = std::__cxx11::to_string(specularNr++);
        // 赋值
        glUniform1i(glGetUniformLocation(shader.Program, (name+idx).c_str()), i);
        // 绑定
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }

    // 绘制Mesh
    shader.Use();
    //  cout << VAO << ' ' << VBO << ' ' << EBO << endl;
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 30);
    glBindVertexArray(0);

    // 好习惯，把使用完把状态复原
    for(GLuint i = 0; i < this->textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}



















