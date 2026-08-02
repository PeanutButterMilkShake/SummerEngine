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
GLuint Engine::whiteTextureId = 0;
EngineUI* Engine::engineUI = nullptr;

Engine::Engine(string name)
{
    Initialize(name);
}

Engine::~Engine()
{

}

void APIENTRY Engine::OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;

    std::cerr << "\n----------------OPENGL ERROR----------------" << std::endl;
    std::cerr << "Message: " << message << std::endl;
    
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:               std::cerr << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Type: Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cerr << "Type: Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cerr << "Type: Portability Issue"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cerr << "Type: Performance Warning"; break;
        default:                                std::cerr << "Type: Other"; break;
    }
    std::cerr << "\n--------------------------------------------\n" << std::endl;
}

void Engine::Initialize(string name)
{
    // Initialize OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // REQUIRE A DEBUG CONTEXT FOR AUTOMATIC ERROR LOGGING
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

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

    // OpenGL debugging
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT); 
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        
        // Check if the function pointer is loaded before calling it
        if (glDebugMessageCallback != nullptr) {
            glDebugMessageCallback(OpenGLDebugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
    }

    // Set all callbacks
    glfwSetErrorCallback(ErrorCallback);
    glfwSetFramebufferSizeCallback(window, WindowResizeCallback);

    // Enable OpenGL graphics stuff? idk the word yet
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Input::window = window;
    
    // TEMP CHANGE WHEN ENGINE UI STUFF
    glGenTextures(1, &whiteTextureId);
    glBindTexture(GL_TEXTURE_2D, whiteTextureId);
    unsigned char whitePixel[] = { 255, 255, 255, 255 };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    engineUI = new EngineUI();
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