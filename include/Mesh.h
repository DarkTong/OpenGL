#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// mine
#include "Shader.h"

using namespace std;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};
struct Texture
{
    GLuint id;
    string type;
    aiString path;
};

class Mesh
{
public:
    GLuint VAO, VBO, EBO;
    // 定点信息
    vector<Vertex> vertices;
    // 纹理信息
    vector<Texture> textures;
    // 索引信息
    vector<GLuint> indices;

    Mesh(vector<Vertex> vertices, vector<Texture> textures, vector<GLuint> indices);
    //Mesh(const Mesh &);
    void Draw(Shader shader);
    virtual ~Mesh();

protected:

private:
    void setupMesh();
};

#endif // MESH_H
