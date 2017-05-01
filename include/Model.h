#ifndef MODEL_H
#define MODEL_H

// SOIL
#include <SOIL.h>

// mine
#include "Mesh.h"

class Model
{
public:
    Model(const GLchar *path);
    virtual ~Model();
    void Draw(Shader shader);

protected:

private:
    // 模型数据
    vector<Mesh> meshes;
    string directory;
    // 已加载过的纹理
    vector<Texture> texturesLoads;
    // 函数
    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                         string typeName);
    GLuint TextureFromFile(const char* path, string dir);

};

#endif // MODEL_H
