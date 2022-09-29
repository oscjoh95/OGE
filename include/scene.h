#ifndef SCENE_H
#define SCENE_H

#include <model.h>

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
    Scene() {}

    /**
     * Checks for the closest model in the scene that the ray that goes
     * through the camera and (screenX, screenY) intersects with.
     *
     * @param screenX       The screen x coordinate (from bottom left)
     * @param screenY       The screen y coordinate (from bottom left)
     * @param screenWidth   The width of the screen in pixels
     * @param screenHeight  The height of the screen in pixels
     * @param distance_out  Output for the distance to the closest object.
     *                      Undefined if no object was intersected
     * @param model_out     Output for the closest model that was
     *                      intersected. Undefined if no object was
     *                      intersected
     * 
     * @returns true if a model was intersected, false otherwise
     */
    bool IsRayOBBIntersecting(
        int screenX, int screenY,
        int screenWidth, int screenHeight,
        float& distance_out,
        Model* model_out) {

        // Set variables that are common for all models
        glm::vec3 rayOrigin;
        glm::vec3 rayDir;
        ScreenPosToWorldRay(
            screenX, screenY,
            screenWidth, screenHeight,
            rayOrigin,
            rayDir);

        float minIntersectionDist = 100.0f;
        Model* model = nullptr;

        // Check for each model and pick the closest
        for (int i = 0; i < models.size(); i++){
            float tMin = 0.0f;
            float tMax = 100000.0f;

            // Create OBB for model
            auto modelMatrix = models.at(i).modelMatrix;
            glm::vec3 OBBposition(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);
            glm::vec3 delta = OBBposition - rayOrigin;
            // Max and min coordinates of object
            glm::vec3 aabb_min = models.at(i).model_p->GetMinCoords();
            glm::vec3 aabb_max = models.at(i).model_p->GetMaxCoords();

            // Test with x axis
            {
                glm::vec3 xaxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
                float e = glm::dot(xaxis, delta);
                float f = glm::dot(rayDir, xaxis);

                if (fabs(f) > 0.001f) {
                    float t1 = (e + aabb_min.x) / f;  // "Left" plane
                    float t2 = (e + aabb_max.x) / f;  // "Right" plane

                    // Switch t1 and t2 if t2 is left
                    if (t1 > t2) {
                        float temp = t1;
                        t1 = t2;
                        t2 = temp;
                    }

                    if (t2 < tMax) {
                        tMax = t2;
                    }
                    if (t1 > tMin) {
                        tMin = t1;
                    }

                    if (tMax < tMin) {
                        continue;
                    }
                } else {
                    if (-e + aabb_min.x > 0.0f || -e + aabb_max.x < 0.0f) {
                        continue;
                    }
                }
            }

            // Test with y axis
            {
                glm::vec3 yaxis(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
                float e = glm::dot(yaxis, delta);
                float f = glm::dot(rayDir, yaxis);

                if (fabs(f) > 0.001f) {
                    float t1 = (e + aabb_min.y) / f;  // "Left" plane
                    float t2 = (e + aabb_max.y) / f;  // "Right" plane

                    // Switch t1 and t2 if t2 is left
                    if (t1 > t2) {
                        float temp = t1;
                        t1 = t2;
                        t2 = temp;
                    }

                    if (t2 < tMax) {
                        tMax = t2;
                    }
                    if (t1 > tMin) {
                        tMin = t1;
                    }

                    if (tMax < tMin) {
                        continue;
                    }
                } else {
                    if (-e + aabb_min.y > 0.0f || -e + aabb_max.y < 0.0f) {
                        continue;
                    }
                }
            }

            // Test with z axis
            {
                glm::vec3 zaxis(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);
                float e = glm::dot(zaxis, delta);
                float f = glm::dot(rayDir, zaxis);

                if (fabs(f) > 0.001f) {
                    float t1 = (e + aabb_min.z) / f;  // "Left" plane
                    float t2 = (e + aabb_max.z) / f;  // "Right" plane

                    // Switch t1 and t2 if t2 is left
                    if (t1 > t2) {
                        float temp = t1;
                        t1 = t2;
                        t2 = temp;
                    }

                    if (t2 < tMax) {
                        tMax = t2;
                    }
                    if (t1 > tMin) {
                        tMin = t1;
                    }

                    if (tMax < tMin) {
                        continue;
                    }
                } else {
                    if (-e + aabb_min.z > 0.0f || -e + aabb_max.z < 0.0f) {
                        continue;
                    }
                }
            }
            // We have an intersection
            if (tMin < minIntersectionDist) {
                minIntersectionDist = tMin;
                model = models.at(i).model_p;
            }
        }
        if (model == nullptr) {
            return false;
        }
        distance_out = minIntersectionDist;
        model_out = model;
        return true;
    }

    /**
     * Finds the ray that goes through the camera and (screenX, screenY)
     *
     * @param screenX       The screen x coordinate (from bottom left)
     * @param screenY       The screen y coordinate (from bottom left)
     * @param screenWidth   The width of the screen in pixels
     * @param screenHeight  The height of the screen in pixels
     * @param origin_out    Output for the origin of the ray
     * @param dir_out     Output for the direction of the ray
     * 
     * @returns void
     */
    void ScreenPosToWorldRay (
        int screenX, int screenY,
        int screenWidth, int screenHeight,
        glm::vec3& origin_out,
        glm::vec3& dir_out) {
            // Calculate start and end positions
            glm::vec4 rayStart(
                ((float)screenX/(float)screenWidth - 0.5f) * 2.0f,
                ((float)screenY/(float)screenHeight - 0.5f) * 2.0f,
                -1.0f,
                1.0f);
            glm::vec4 rayEnd(
                ((float)screenX/(float)screenWidth - 0.5f) * 2.0f,
                ((float)screenY/(float)screenHeight - 0.5f) * 2.0f,
                0.0f,
                1.0f);

            // Calculate matrices
            glm::mat4 inverseProjection = glm::inverse(projection);
            glm::mat4 inverseView = glm::inverse(camera->GetViewMatrix());

            // Convert ray to world vector
            glm::vec4 rayStart_camera = inverseProjection * rayStart;
            rayStart_camera /= rayStart_camera.w;
            glm::vec4 rayStart_world = inverseView * rayStart_camera;
            rayStart_world /= rayStart_world.w;
            glm::vec4 rayEnd_camera = inverseProjection * rayEnd;
            rayEnd_camera /= rayEnd_camera.w;
            glm::vec4 rayEnd_world = inverseView * rayEnd_camera;
            rayEnd_world /= rayEnd_world.w;

            // Set output
            origin_out = glm::vec3(rayStart_world);
            dir_out = glm::normalize(rayEnd_world - rayStart_world);
    }

    /**
     * Sets view projection and model matrices and draws all models
     * in the scene.
     * 
     * @returns void
     */
    void Draw() {
        for (auto modelData : models) {
            // Set matrices and draw
            modelData.shader_p->use();
            modelData.shader_p->setMat4("view", view);
            modelData.shader_p->setMat4("projection", projection);
            modelData.shader_p->setMat4("model", modelData.modelMatrix);
            modelData.model_p->Draw(*modelData.shader_p);
        }
    }

    /**
     * Creates a model data struct and adds it to the vector of model data
     *
     * @param model_p A pointer to the model object
     * @param modelMatrix The model matrix
     * @param shader_p A pointer to the shader program to use when rendering
     * 
     * @returns void
     */
    void AddModel(Model* model_p, glm::mat4 modelMatrix, Shader* shader_p) {
        models.push_back({model_p, modelMatrix, shader_p});
    }

    /**
     * Sets the camera to use for rendering the scene
     *
     * @param cam A pointer to the camera object
     * 
     * @returns void
     */
    void SetCamera(Camera* cam) {
        camera = cam;
    }

    /**
     * updates the view and porjection matrices for the scene
     *
     * @param screenWidth   The width of the screen in pixels
     * @param screenHeight  The height of the screen in pixels
     * 
     * @returns void
     */
    void UpdateMatrices(int screenWidth, int screenHeight) {
        view = camera->GetViewMatrix();
        projection = glm::perspective(
            glm::radians(camera->Zoom),
            (float)screenWidth / (float)screenHeight,
            0.1f, 100.0f);
    }

 private:
    std::vector<ModelData> models;
    Camera* camera;
    glm::mat4 projection;
    glm::mat4 view;
};

#endif  // SCENE_H