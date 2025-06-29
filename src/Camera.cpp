#include "Camera.h"

Camera::Camera(glm::vec3 startPosition, glm::vec3 up, float startYaw, float startPitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(0.05f), sensitivity(0.1f)
{
    position = startPosition;
    worldUp = up;
    yaw = startYaw;
    pitch = startPitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(char direction) {
    if (direction == 'W')
        position += front * speed;
    if (direction == 'S')
        position -= front * speed;
    if (direction == 'A')
        position -= right * speed;
    if (direction == 'D')
        position += right * speed;
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(f);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}