#ifndef SCENE_H
#define SCENE_H

#include <model.h>
#include <camera.h>

#include <vector>
#include <iostream>

struct ModelData {
    Model* model_p;
    glm::mat4 modelMatrix;
    Shader* shader_p;
};

class Scene {
 public:
    // Constructor
    Scene() = default;

    // Looks for the closest OBB that is intersected 
    // with a ray through screenX, screenY
    bool IsRayOBBIntersecting(
        int screenX, int screenY,
        int screenWidth, int screenHeight,
        float& distance_out,
        Model* model_out);
    
    // Calculates the ray through screenX, screenY
    void ScreenPosToWorldRay (
        int screenX, int screenY,
        int screenWidth, int screenHeight,
        glm::vec3& origin_out,
        glm::vec3& dir_out);

    // Renders the scene
    void Draw();

    // Adds a model to the scene
    void AddModel(Model* model_p, glm::mat4 modelMatrix, Shader* shader_p);

    // Sets the scene's camera
    void SetCamera(Camera* cam);

    // Updates the view and projection matrices
    void UpdateMatrices(int screenWidth, int screenHeight);

 private:
    std::vector<ModelData> models;
    Camera* camera;
    glm::mat4 projection;
    glm::mat4 view;
};

#endif  // SCENE_H