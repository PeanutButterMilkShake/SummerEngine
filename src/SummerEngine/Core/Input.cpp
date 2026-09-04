#include "Input.h"

GLFWwindow *Input::window = nullptr;

Vector2 scrollDelta;
int currentKey = GLFW_KEY_UNKNOWN;
std::string currentPrintableKey = "";

std::string codepoint_to_utf8(unsigned int codepoint) 
{
    std::string utf8;
    if (codepoint <= 0x7F) {
        utf8 += static_cast<char>(codepoint);
    } else if (codepoint <= 0x7FF) {
        utf8 += static_cast<char>(0xC0 | ((codepoint >> 6) & 0x1F));
        utf8 += static_cast<char>(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0xFFFF) {
        utf8 += static_cast<char>(0xE0 | ((codepoint >> 12) & 0x0F));
        utf8 += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        utf8 += static_cast<char>(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0x10FFFF) {
        utf8 += static_cast<char>(0xF0 | ((codepoint >> 18) & 0x07));
        utf8 += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
        utf8 += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        utf8 += static_cast<char>(0x80 | (codepoint & 0x3F));
    }
    return utf8;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    scrollDelta.x = xoffset;
    scrollDelta.y = yoffset;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
        currentKey = key;
    }
    if(action == GLFW_RELEASE && key == currentKey)
    {
        currentKey = GLFW_KEY_UNKNOWN;
    }
}


void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    currentPrintableKey += codepoint_to_utf8(codepoint);
    std::cout << currentPrintableKey << std::endl;
}

void Input::SetupInputCallbacks()
{
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, character_callback);
}

void Input::Update()
{
    scrollDelta = {0,0};
    currentPrintableKey = "";
}

bool Input::IsKeyDown(KeyCode keyCode)
{
    return glfwGetKey(window, KeyCodeToGLFW(keyCode)) == GLFW_PRESS;
}

KeyCode Input::GetCurrentKey()
{
    return static_cast<KeyCode>(currentKey);
}

std::string Input::GetCurrentPrintableKey()
{
    return currentPrintableKey;     
}

std::string Input::GetKeyName(KeyCode keyCode)
{
    const char* name = glfwGetKeyName(KeyCodeToGLFW(keyCode), 0);
    if (name)
    {
        std::string s(name);
        s[0] = std::toupper(s[0]);
        return s;
    }

    switch (keyCode)
    {
        case KeyCode::UpArrow:     return "Up Arrow";
        case KeyCode::LeftArrow:   return "Left Arrow";
        case KeyCode::DownArrow:   return "Down Arrow";
        case KeyCode::RightArrow:  return "Right Arrow";
        case KeyCode::Space:       return "Space";
        case KeyCode::LeftShift:   return "Left Shift";
        case KeyCode::LeftControl: return "Left Control";
        default:                   return "";
    }
}

int Input::GetInputAxis(KeyCode negative, KeyCode positive)
{
    int value = 0;
    if(Input::IsKeyDown(negative))
        value = -1;
    if(Input::IsKeyDown(positive))
        value = 1;

    return value;
}

glm::vec2 Input::GetInputVector2(KeyCode negativeX, KeyCode positiveX, KeyCode negativeY, KeyCode positiveY)
{
    glm::vec2 inputVector = glm::vec2(0);
    inputVector.x = GetInputAxis(negativeX, positiveX);
    inputVector.y = GetInputAxis(negativeY, positiveY);

    return inputVector;
}

int Input::KeyCodeToGLFW(KeyCode keycode)
{
    return static_cast<int>(keycode);
}

Vector2 Input::GetMousePosition()
{
    double x,y;
    glfwGetCursorPos(window, &x, &y);

    return Vector2(x,y);
}

bool Input::GetMouseButtonDown(int button)
{
    return glfwGetMouseButton(window, button);
}

bool Input::GetMouse0Down()
{
    return glfwGetMouseButton(window, 0);
}

bool Input::GetMouse1Down()
{
    return glfwGetMouseButton(window, 0);
}

Vector2 Input::GetScrollDirection()
{
    return scrollDelta;
}

KeyCode Input::GetPressedKey()
{
    if(currentKey == GLFW_KEY_UNKNOWN)
    {
        return KeyCode::NONE;
    }
    else
    {
        return static_cast<KeyCode>(currentKey);
    }
}