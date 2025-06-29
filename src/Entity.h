#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Entity
{
public:
    Entity(float x, float y, float z,
           glm::vec3 baseColor,
           float initialScale,
           const std::string &objFilePath,
           const std::string &mtlFilePath,
           const std::string &textureFilePath,
           glm::vec3 initialRotation = glm::vec3(0.0f));

    bool followBezier = false;

    void initialize();
    void draw(const glm::vec3 &lightPosition);
    void setViewProjection(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &cameraPosition);

    void toggleRotateX();
    void toggleRotateY();
    void toggleRotateZ();

    void scaleUp();
    void scaleDown();

    void moveForward();
    void moveBackward();

    void loadBezierControlPoints(const std::string &file);
    void updateBezierTrajectory();

private:
    glm::vec3 position;
    glm::vec3 rotation;

    glm::vec3 baseColor;
    glm::vec3 initialRotation;
    float scaleFactor;

    bool rotateX, rotateY, rotateZ;

    GLuint VAO;
    GLuint textureID;
    int nVertices;
    GLuint shaderProgram;

    std::string objFilePath;
    std::string mtlFilePath;
    std::string textureFilePath;

    float ka, kd, ks, shininess;

    std::vector<glm::vec3> bezierControlPoints;
    std::vector<glm::vec3> bezierRotations;

    float bezierT = 0.0f;
    float bezierSpeed = 0.001f;

    int loadModelWithTexture(const std::string &objFilePath,
                             const std::string &mtlFilePath,
                             const std::string &textureFilePath,
                             int &outVertices,
                             GLuint &outTextureID);

    void loadMaterial(const std::string &mtlFilePath);
    GLuint loadTexture(const std::string &texturePath);

    void setupShaders();
    void checkCompileErrors(GLuint shader, std::string type);

    static constexpr float TRANSLATION_SPEED = 0.1f;
};

#endif