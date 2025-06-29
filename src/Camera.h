#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float fov = 100.0f;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    Camera(glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float startYaw = -90.0f,
           float startPitch = 0.0f);

    glm::mat4 getViewMatrix();

    void processKeyboard(char direction);
    void processMouseMovement(float xoffset, float yoffset);

private:
    void updateCameraVectors();
};