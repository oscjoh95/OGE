#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model {
 public:
    // Constructor with path to the model dir 
    Model(std::string const& path);

    // Renders the model
    void Draw(Shader& shader);

    // Gets max coordinate in each direction
    glm::vec3 GetMaxCoords();

    // Gets min coordinate in each direction
    glm::vec3 GetMinCoords();

 private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> loaded_textures;
    glm::vec3 aabb_max = glm::vec3(-1000.0f, -1000.0f, -1000.0f);
    glm::vec3 aabb_min = glm::vec3(1000.0f, 1000.0f, 1000.0f);

    // Loads a model from specified path
    void loadModel(std::string path);

    // Recursively processes all the child nodes and meshes of a node
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char *path, const std::string &directory);
};

#endif  // MODEL_H