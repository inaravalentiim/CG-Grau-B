#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Entity.h"
#include "Camera.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

const GLuint WIDTH = 1000, HEIGHT = 1000;
int selectedEntityIndex = 0;
std::vector<Entity> entities;

Camera camera;
glm::vec3 lightPos;

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void loadSceneFromJSON(const std::string &jsonFile);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "GB - Inara Valentim :)", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    loadSceneFromJSON("../assets/scene.json");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        for (auto &entity : entities)
        {
        
            entity.updateBezierTrajectory();
            entity.setViewProjection(view, projection, camera.position);
            entity.draw(lightPos);
        }

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_W)
            camera.processKeyboard('W');
        if (key == GLFW_KEY_S)
            camera.processKeyboard('S');
        if (key == GLFW_KEY_A)
            camera.processKeyboard('A');
        if (key == GLFW_KEY_D)
            camera.processKeyboard('D');

        Entity &selectedEntity = entities[selectedEntityIndex];

        if (key == GLFW_KEY_X)
            selectedEntity.toggleRotateX();
        if (key == GLFW_KEY_Y)
            selectedEntity.toggleRotateY();
        if (key == GLFW_KEY_Z)
            selectedEntity.toggleRotateZ();

        if (key == GLFW_KEY_E)
            selectedEntity.scaleUp();
        if (key == GLFW_KEY_Q)
            selectedEntity.scaleDown();

        if (key == GLFW_KEY_U)
            selectedEntity.moveForward();
        if (key == GLFW_KEY_J)
            selectedEntity.moveBackward();

        if (key == GLFW_KEY_C)
            selectedEntityIndex = (selectedEntityIndex + 1) % entities.size();
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void loadSceneFromJSON(const std::string &jsonFile)
{
    std::ifstream file(jsonFile);
    if (!file.is_open())
    {
        std::cerr << "Failed to open JSON scene file: " << jsonFile << std::endl;
        return;
    }

    json scene;
    file >> scene;

    camera.position = glm::vec3(scene["camera"]["position"][0], scene["camera"]["position"][1], scene["camera"]["position"][2]);
    camera.front = glm::vec3(scene["camera"]["front"][0], scene["camera"]["front"][1], scene["camera"]["front"][2]);
    camera.up = glm::vec3(scene["camera"]["up"][0], scene["camera"]["up"][1], scene["camera"]["up"][2]);
    camera.fov = scene["camera"]["fov"];

    lightPos = glm::vec3(scene["light"]["position"][0], scene["light"]["position"][1], scene["light"]["position"][2]);

    for (const auto &obj : scene["entities"])
    {
        Entity entity(
            obj["position"][0],
            obj["position"][1],
            obj["position"][2],
            glm::vec3(0.0f, 0.0f, 0.0f),
            obj["scale"],
            obj["obj"],
            obj["mtl"],
            obj["texture"],
            glm::vec3(
                glm::radians(float(obj["rotation"][0])),
                glm::radians(float(obj["rotation"][1])),
                glm::radians(float(obj["rotation"][2]))));

        if (!obj["trajectory"].get<std::string>().empty())
        {
            entity.loadBezierControlPoints(obj["trajectory"]);
        }
        entity.initialize();
        entities.push_back(entity);
    }
}