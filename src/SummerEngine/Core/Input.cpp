#include "Input.h"

GLFWwindow *Input::window = nullptr;

bool Input::IsKeyDown(KeyCode keyCode)
{
    return glfwGetKey(window, KeyCodeToGLFW(keyCode)) == GLFW_PRESS;
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

int Input::KeyCodeToGLFW(KeyCode keycode) //make this better bruh what is this
{
    switch (keycode)
    {
    case KeyCode::A: return GLFW_KEY_A;
    case KeyCode::B: return GLFW_KEY_B;
    case KeyCode::C: return GLFW_KEY_C;
    case KeyCode::D: return GLFW_KEY_D;
    case KeyCode::E: return GLFW_KEY_E;
    case KeyCode::F: return GLFW_KEY_F;
    case KeyCode::G: return GLFW_KEY_G;
    case KeyCode::H: return GLFW_KEY_H;
    case KeyCode::I: return GLFW_KEY_I;
    case KeyCode::J: return GLFW_KEY_J;
    case KeyCode::K: return GLFW_KEY_K;
    case KeyCode::L: return GLFW_KEY_L;
    case KeyCode::M: return GLFW_KEY_M;
    case KeyCode::N: return GLFW_KEY_N;
    case KeyCode::O: return GLFW_KEY_O;
    case KeyCode::P: return GLFW_KEY_P;
    case KeyCode::Q: return GLFW_KEY_Q;
    case KeyCode::R: return GLFW_KEY_R;
    case KeyCode::S: return GLFW_KEY_S;
    case KeyCode::T: return GLFW_KEY_T;
    case KeyCode::U: return GLFW_KEY_U;
    case KeyCode::V: return GLFW_KEY_V;
    case KeyCode::W: return GLFW_KEY_W;
    case KeyCode::X: return GLFW_KEY_X;
    case KeyCode::Y: return GLFW_KEY_Y;
    case KeyCode::Z: return GLFW_KEY_Z;

    case KeyCode::UpArrow: return GLFW_KEY_UP;
    case KeyCode::LeftArrow: return GLFW_KEY_LEFT;
    case KeyCode::DownArrow: return GLFW_KEY_DOWN;
    case KeyCode::RightArrow: return GLFW_KEY_RIGHT;

    case KeyCode::Space: return GLFW_KEY_SPACE;
    case KeyCode::LeftShift: return GLFW_KEY_LEFT_SHIFT;
    case KeyCode::LeftControl: return GLFW_KEY_LEFT_CONTROL;

    default: return GLFW_KEY_UNKNOWN;
    }
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