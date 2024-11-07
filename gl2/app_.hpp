#ifndef APP__HPP
#define APP__HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <algorithm>
#include <utility>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vao.hpp"
#include "vbo.hpp"
#include "ebo.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "GLC.hpp"

#define FPS 120

class App {
private:
    GLFWwindow* window;
    int width;
    int height;
    double curTime;
    double lastTime;
    float deltaTime;
    float fpsDelay;

public:
    App(void);
    ~App(void);
    GLFWwindow* getWindow(void);
    int getWidth(void);
    int getHeight(void);

private:
    void updatewh(void);
    void updateDeltaTime(void);
    bool shouldRenderFrame(void);
    void clearBackground(void);
    void createWindow(void);
    void processInput(void);
    void the_main(void);
};

#endif /* APP__HPP */