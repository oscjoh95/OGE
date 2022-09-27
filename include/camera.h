#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera {
 public:
    // Camera attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler angles
    float Yaw;
    float Pitch;

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor with vectors
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH
    ) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Constructor with float values
    Camera(
        float posX, float posY, float posZ,
        float upX, float upY, float upZ,
        float yaw, float pitch
    ) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM) {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    /**
     * Getter for the view matrix for the current camera settings.
     * 
     * @returns A mat4 containing the view transform matrix
     */
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    /**
     * Processes input from the keyboard to move the camera consistently
     * even with different frame lengths.
     *
     * @param direction The direction to move the camera
     * @param deltaTime The length of the frame in time
     * 
     * @returns void
     */
    void ProcessKeyboard (Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD) {
            Position += Front * velocity;
        }
        if (direction == BACKWARD) {
            Position -= Front * velocity;
        }
        if (direction == LEFT) {
            Position -= Right * velocity;
        }
        if (direction == RIGHT) {
            Position += Right * velocity;
        }
        if (direction == UP) {
            Position += WorldUp * velocity;
        }
        if (direction == DOWN) {
            Position -= WorldUp * velocity;
        }
    }

    /**
     * Processes input from the mouse and rotates the camera accordingly
     *
     * @param xoffset The offset in x direction
     * @param yoffset The offset in y direction
     * @param constrainPitch Set to true to constrain the pitch to +-90
     *                      degrees. Default true
     *                       
     * 
     * @returns void
     */
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // Make sure the camera doesn't get flipped
        if (constrainPitch) {
            if (Pitch > 89.0f) {
                Pitch = 89.0f;
            }
            if (Pitch < -89.0f) {
                Pitch = -89.0f;
            }
        }

        // Update the camera vectors now that we changed the yaw and pitch
        updateCameraVectors();
    }

    /**
     * Processess input from scroll wheel to zoom.
     *
     * @param yoffset The offset of the scroll wheel.
     * 
     * @returns void
     */
    void ProcessMouseScroll(float yoffset) {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f) {
            Zoom = 1.0f;
        }
        if (Zoom > 45.0f) {
            Zoom = 45.0f;
        }
    }

 private:
    /**
     * Updates the camera vectors.
     * 
     * @returns void
     */
    void updateCameraVectors() {
        // Calculate new front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Calculate new right vector
        Right = glm::normalize(glm::cross(Front, WorldUp));
        // Calculate new up vector
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

#endif  // CAMERA_H