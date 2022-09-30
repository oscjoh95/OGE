#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>
#include <model.h>
#include <scene.h>

#include <iostream>
#include <filesystem>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow* window);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX =  SCR_WIDTH / 2.0;
float lastY =  SCR_HEIGHT / 2.0;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Clicking
bool markObject = false; 

int main(int argc, char** argv) { 
    // Initialize and configure glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Glfw window creation
    GLFWwindow* window = glfwCreateWindow(800, 600, "OGE Sample Program", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Input callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Capture mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Flip loaded textures on y-axis before loading model
    stbi_set_flip_vertically_on_load(true);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Build and compile the shader program
    std::string dir = std::filesystem::weakly_canonical(std::filesystem::path(argv[0])).parent_path().string();
    Shader modelShader((dir + "/shaders/light_shader.vs").c_str(), (dir + "/shaders/light_shader.fs").c_str());
    
    // Load the model
    Model model(dir + "/resources/objects/backpack/backpack.obj");
    glm::mat4 modelMat = glm::mat4(1.0f);
    Scene scene;
    scene.AddModel(&model, modelMat, &modelShader);
    scene.SetCamera(&camera);

    // Uncomment to set wireframe mode on
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Activate shader
        modelShader.use();

        // Set lighting params
        modelShader.setVec3("viewPos", camera.Position);
        modelShader.setFloat("material.shininess", 0.3f);
        modelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        modelShader.setVec3("dirLight.ambient", glm::vec3(0.05f));
        modelShader.setVec3("dirLight.diffuse", glm::vec3(0.4f));
        modelShader.setVec3("dirLight.specular", glm::vec3(0.5f));

        modelShader.setVec3("pointLights[0].position", 0.0f, 0.0f, 3.0f);
        modelShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[0].constant", 1.0f);
        modelShader.setFloat("pointLights[0].linear", 0.09f);
        modelShader.setFloat("pointLights[0].quadratic", 0.032f);

        // Update matrices in the scene
        scene.UpdateMatrices(SCR_WIDTH, SCR_HEIGHT);

        // Check if any object are clicked
        if (markObject) {
            float dist;
            double xPos, yPos;
            glfwGetCursorPos(window, &xPos, &yPos);
            Model* clickedObject;
            if (scene.IsRayOBBIntersecting(
                (float)xPos, (float)(SCR_HEIGHT - yPos),
                SCR_WIDTH, SCR_HEIGHT,
                dist,
                clickedObject)) {
                    std::cout << clickedObject << " was clicked at distance " << dist << std::endl;
                } else {
                    std::cout << "No object" << std::endl;
                }
            markObject = false;
        }

        // Draw scene
        scene.Draw();

        // Swap buffers and poll for IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Deallocate all allocated glfw resources
    glfwTerminate();
    return 0;
}

// Process input
void processInput(GLFWwindow* window) {
    // Exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Camera controls
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
}

// Callback for mouse movement
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    // Cast input to float
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    // Set values if this is the first call
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Calculate offset
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    // Store values for next time this function is called
    lastX = xpos;
    lastY = ypos;

    // Set new camera position
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// Callback for mouse scroll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// Callback function for window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        markObject = true;
}
