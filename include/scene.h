#ifndef SCENE_H
#define SCENE_H

#include <model.h>
#include <camera.h>

#include <vector>
#include <iostream>

template<typename T>
struct UniformData {
    std::string name;
    T value;
};

struct ModelData {
    Model* model_p;
    glm::mat4 modelMatrix;
    Shader* shader_p;
    std::vector<UniformData<glm::vec3>> vec3_uniforms;
};

class Scene {
 public:
    // Constructor
    Scene() = default;

    // Check for intersection of a ray with the bounding box of an object
    bool IsRayOBBIntersecting(
        glm::vec3 rayOrigin,
        glm::vec3 rayDir,
        glm::mat4 modelMatrix,
        glm::vec3 aabb_min,
        glm::vec3 aabb_max,
        float& distance_out);
    
    // Calculates the ray through screenX, screenY
    void ScreenPosToWorldRay (
        int screenX, int screenY,
        int screenWidth, int screenHeight,
        glm::vec3& origin_out,
        glm::vec3& dir_out);

    // Renders the scene
    void Draw();

    // Adds a model to the scene
    void AddModel(
        Model* model_p,
        glm::mat4 modelMatrix,
        Shader* shader_p,
        std::vector<UniformData<glm::vec3>> vec3_uniforms = {});

    // Clears the vector of model data
    void ClearModels();

    // Sets the scene's camera
    void SetCamera(Camera* cam);

    // Updates the view and projection matrices
    void UpdateMatrices(int screenWidth, int screenHeight);

 private:
    std::vector<ModelData> models;
    Camera* camera;
    glm::mat4 projection;
    glm::mat4 view;

    // Sets uniforms for a model
    void SetModelUniforms(Shader* shader_p, std::vector<UniformData<glm::vec3>> vec3_uniforms);
};

#endif  // SCENE_H