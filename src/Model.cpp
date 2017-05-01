#include "Model.h"

Model::Model(const GLchar* path)
{
    //ctor
    this->loadModel(path);
}

Model::~Model()
{
    //dtor
}

// 绘图
void Model::Draw(Shader shader)
{
    for(GLuint i = 0;i < this->meshes.size(); ++i)
        this->meshes[i].Draw(shader);
}

// 加载模型
void Model::loadModel(string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP" << importer.GetErrorString() << endl;
        return;
    }
    this->directory = path.substr(0, path.find_last_of('/'));

    this->processNode(scene->mRootNode, scene);
}

// 处理场景节点
void Model::processNode(aiNode* node, const aiScene* scene)
{
    // 添加当前节点中所有Mesh
    for(GLuint i=0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(this->processMesh(mesh, scene));
    }
    // 递归处理子节点
    for(GLuint i = 0; i < node->mNumChildren; ++i)
    {
        this->processNode(node->mChildren[i], scene);
    }
}

// 将Assimp的mesh数据转化为我们定义的mesh数据
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // 我们定义的mesh基本数据
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    for(GLuint i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;
        // 处理定点坐标、法线和纹理坐标
        glm::vec3 __vector__;
        // -定点坐标
        __vector__.x = mesh->mVertices[i].x;
        __vector__.y = mesh->mVertices[i].y;
        __vector__.z = mesh->mVertices[i].z;
        vertex.Position = __vector__;
        // -法线向量
        __vector__.x = mesh->mNormals[i].x;
        __vector__.y = mesh->mNormals[i].y;
        __vector__.z = mesh->mNormals[i].z;
        vertex.Normal = __vector__;
        // -纹理坐标
        if(mesh->mTextureCoords[0]) //判断是否存在纹理坐标
        {
            __vector__.x = mesh->mTextureCoords[0][i].x;
            __vector__.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = glm::vec2(__vector__.x, __vector__.y);
        }
        else
            vertex.TexCoords = glm::vec2(0.0, 0.0);

        vertices.push_back(vertex);
    }
    // 处理定点索引
    for(GLuint i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for(GLuint j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }
    // 处理材质
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps = this->loadMaterialTextures(material,
                                            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps = this->loadMaterialTextures(material,
                                            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, textures, indices);
}

// 获取纹理信息
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    for(GLuint i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        GLboolean skip = GL_FALSE;
        for(GLuint j = 0; j < this->texturesLoads.size(); ++j)
        {
            if(std::strcmp(this->texturesLoads[j].path.C_Str(), str.C_Str()) == 0)
            {
                textures.push_back(this->texturesLoads[j]);
             //   skip = GL_TRUE;
             //   break;
            }
        }
        if(skip == GL_FALSE)
        {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
            this->texturesLoads.push_back(texture);
        }
    }
    return textures;
}

// 获取纹理对象id（同事加载）
GLuint Model::TextureFromFile(const char* path, string directory)
{
    // 获取纹理的绝对路径
    string fileName = string(path);
    fileName = directory + '/' + fileName;
    cout << fileName << endl;

    unsigned char* image;
    int width, height;
    GLuint maps;
    // 生成纹理对象
    glGenTextures(1, &maps);
    // 绑定
    glBindTexture(GL_TEXTURE_2D, maps);
    // 获取图片
    image = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    // 加载纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    // 释放图片内存
    //SOIL_free_image_data(image);
    // 生成多级纹理
    glGenerateMipmap(GL_TEXTURE_2D);
    // 设置纹理属性
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    // 解绑
    glBindTexture(GL_TEXTURE_2D, 0);

    // 释放图片内存
    SOIL_free_image_data(image);
    return maps;
}
