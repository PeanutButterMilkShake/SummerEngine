#include "Engine.h"
#include "Object.h"

std::vector<Object*> Engine::objects;
Camera* Engine::mainCamera = nullptr;

Engine::Engine(string name)
{
    Initialize(name);
}

Engine::~Engine()
{

}
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void Engine::Initialize(string name)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, name.c_str(), NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    Input::window = window;
}

void Engine::Update()
{
    for (Object *object : Engine::objects)
        object->_SceneUpdate();
}