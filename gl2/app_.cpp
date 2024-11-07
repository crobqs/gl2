#include "app_.hpp"



static float aspectRatio = 0.0f;
float* deltaTime_ptr = nullptr;
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static float lastX;
static float lastY;
static bool firstMouse = true;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

App::App(void) : window(NULL), width(0), height(0), curTime(0), lastTime(0), deltaTime(0)
{
    fpsDelay = 1.0f / FPS;
    deltaTime_ptr = &deltaTime;
    createWindow();
    the_main();
}

App::~App(void) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* App::getWindow(void) { return this->window; }

int App::getWidth(void) {
    glfwGetFramebufferSize(window, &width, &height);
    return width;
}

int App::getHeight(void) {
    glfwGetFramebufferSize(window, &width, &height);
    return height;
}

void App::updatewh(void) {
    glfwGetFramebufferSize(window, &width, &height);
}

void App::updateDeltaTime(void) {
    curTime = glfwGetTime();
    deltaTime = static_cast<float>(curTime - lastTime);
}

bool App::shouldRenderFrame(void) {
    if (deltaTime >= fpsDelay) {
        lastTime = curTime;
        return true;
    }
    return false;
}

void App::clearBackground(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/* >>> STATIC FUNCTIONS BEGIN >>> */

static void framebuffersizeCallback(GLFWwindow* window_, int width_, int height_) {
    (void)window_;
    glViewport(0, 0, width_, height_);
    aspectRatio = width_ / static_cast<float>(height_);
}

static void cursorposCallback(GLFWwindow* window_, double xposIn, double yposIn) {
    (void)window_;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    float timeFactor = std::max(*deltaTime_ptr, 0.01f);  // Prevent very small delta
    xoffset *= timeFactor;
    yoffset *= timeFactor;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

static void scrollCallback(GLFWwindow* window_, double xoffset, double yoffset) {
    (void)window_;
    (void)xoffset;
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length,
    const GLchar* message, const void* userParam) {
    (void)length; // Unused parameter
    (void)userParam; // Unused parameter

    // Map the source to a readable string
    const char* sourceStr;
    switch (source) {
    case GL_DEBUG_SOURCE_API:             sourceStr = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceStr = "Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceStr = "Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     sourceStr = "Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           sourceStr = "Other"; break;
    default:                              sourceStr = "Unknown"; break;
    }

    // Map the type to a readable string
    const char* typeStr;
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:               typeStr = "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated Behavior"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeStr = "Undefined Behavior"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         typeStr = "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         typeStr = "Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              typeStr = "Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          typeStr = "Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           typeStr = "Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               typeStr = "Other"; break;
    default:                                typeStr = "Unknown"; break;
    }

    // Map the severity to a readable string
    const char* severityStr;
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:         severityStr = "High"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       severityStr = "Medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          severityStr = "Low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "Notification"; break;
    default:                             severityStr = "Unknown"; break;
    }

    // Print the complete debug message
    std::cerr << "GL CALLBACK:\n"
        << "  Message: " << message << "\n"
        << "  Source: " << sourceStr << "\n"
        << "  Type: " << typeStr << "\n"
        << "  Severity: " << severityStr << "\n"
        << "  ID: " << id << std::endl;
}


/* <<< STATIC FUNCTIONS END <<< */

void App::createWindow(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    bool fullscreen = true; // Configurable at runtime
    if (fullscreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        window = glfwCreateWindow(mode->width, mode->height, "OpenGL", monitor, NULL);
    }
    else {
        width = 800;
        height = 600;
        window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(window, framebuffersizeCallback);
    glEnable(GL_DEPTH_TEST);
    updatewh();
    glViewport(0, 0, width, height);
    glfwSetCursorPosCallback(window, cursorposCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, nullptr);

    lastX = width / static_cast<float>(2);
    lastY = height / static_cast<float>(2);
    aspectRatio = width / static_cast<float>(height);
}

void App::processInput(void) {
    // close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // polygon mode (line, fill)
    static bool prevPolygonState = false;
    bool currentPolygonState = glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS;
    if (currentPolygonState && !prevPolygonState) {
        int polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        if (polygonMode[0] == GL_FILL && polygonMode[1] == GL_FILL)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else if (polygonMode[0] == GL_LINE && polygonMode[1] == GL_LINE)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    prevPolygonState = currentPolygonState;
    // cursor disabled/normal
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // movement controls
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camera.ProcessKeyboard(DOWN, deltaTime);
}

void App::the_main(void) {
    Shader shader("cube.shader");
    Shader lightShader("lightcube.shader");
    std::vector<float> vertices = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    VBO vbo(vertices);

    VAO vao;
    vao.addAttrib(vbo.getId(), 0, 3, 8, 0);
    vao.addAttrib(vbo.getId(), 1, 3, 8, 3);
    vao.addAttrib(vbo.getId(), 2, 2, 8, 6);
    shader.setVao(vao.getId());

    VAO lightVao;
    lightVao.addAttrib(vbo.getId(), 0, 3, 8, 0);
    lightShader.setVao(lightVao.getId());

    Texture tex1_diffuseMap("resources/textures/container2.png", GL_TEXTURE0);
    Texture tex2_specularMap("resources/textures/container2_specular.png", GL_TEXTURE1);
    shader.bind();
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);
    shader.unbind();

    deltaTime = 0;
    lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        updateDeltaTime();
        glfwPollEvents();
        if (shouldRenderFrame()) {
            processInput();
            // draw cube
            shader.bind();
            tex1_diffuseMap.bind();
            tex2_specularMap.bind();
            shader.setVec3("light.position", lightPos);
            shader.setVec3("viewPos", camera.Position);
            shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
            shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
            shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
            shader.setFloat("material.shininess", 64.0f);
            glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), aspectRatio, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 model = glm::mat4(1.0f);
            shader.setMat4("projection", proj);
            shader.setMat4("view", view);
            shader.setMat4("model", model);
            clearBackground();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            tex1_diffuseMap.unbind();
            tex2_specularMap.unbind();
            shader.unbind();

            // draw lamp
            lightShader.bind();
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader.setMat4("mvp", proj * view * model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            lightShader.unbind();

            glfwSwapBuffers(window);
        }
    }
}