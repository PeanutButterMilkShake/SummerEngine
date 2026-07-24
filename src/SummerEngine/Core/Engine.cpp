#include "Engine.h"
#include "Object.h"
#include "Renderer.h"
#include "Light.h"

std::vector<Object*> Engine::objects;
std::vector<Light*> Engine::lights;
Camera* Engine::mainCamera = nullptr;
float Engine::delta;
int Engine::fps;
Vector2 Engine::windowDimensions = Vector2(960, 600);

Engine::Engine(string name)
{
    Initialize(name);
}

Engine::~Engine()
{

}

void Engine::Initialize(string name)
{
    // Initialize OpenGL version 3.3
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open window
    window = glfwCreateWindow(windowDimensions.x, windowDimensions.y, name.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    // Load GLAD functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Set all callbacks
    glfwSetErrorCallback(ErrorCallback);
    glfwSetFramebufferSizeCallback(window, WindowResizeCallback);

    // Enable OpenGL graphics stuff? idk the word yet
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Input::window = window;
}

void Engine::CalculateTimeData()
{
    float currentFrame = static_cast<float>(glfwGetTime());
    delta = currentFrame - lastFrame;
    lastFrame = currentFrame;

    static float fpsTimer = 0.0f;
    static int frameCount = 0;

    fpsTimer += delta;
    frameCount++;

    if (fpsTimer >= 1.0f)
    {
        fps = frameCount;
        frameCount = 0;
        fpsTimer = 0.0f;

        std::string title = "Engine | FPS: " + std::to_string(fps);
        glfwSetWindowTitle(window, title.c_str());
    } 
}

void Engine::ResetTime()
{
    lastFrame = static_cast<float>(glfwGetTime());
}

void Engine::Update()
{
    CalculateTimeData();

    for (Object *object : Engine::objects)
        object->Update(delta);

    Renderer::Render();
}

void Engine::ErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

// Window resize callback function
void Engine::WindowResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Engine::windowDimensions = Vector2(width, height);
}