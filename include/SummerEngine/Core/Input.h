#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "MathTypes.h"

enum KeyCode
{
    A = GLFW_KEY_A,
    B = GLFW_KEY_B,
    C = GLFW_KEY_C,
    D = GLFW_KEY_D,
    E = GLFW_KEY_E,
    F = GLFW_KEY_F,
    G = GLFW_KEY_G,
    H = GLFW_KEY_H,
    I = GLFW_KEY_I,
    J = GLFW_KEY_J,
    K = GLFW_KEY_K,
    L = GLFW_KEY_L,
    M = GLFW_KEY_M,
    N = GLFW_KEY_N,
    O = GLFW_KEY_O,
    P = GLFW_KEY_P,
    Q = GLFW_KEY_Q,
    R = GLFW_KEY_R,
    S = GLFW_KEY_S,
    T = GLFW_KEY_T,
    U = GLFW_KEY_U,
    V = GLFW_KEY_V,
    W = GLFW_KEY_W,
    X = GLFW_KEY_X,
    Y = GLFW_KEY_Y,
    Z = GLFW_KEY_Z,
    Period = GLFW_KEY_PERIOD,

    UpArrow = GLFW_KEY_UP,
    LeftArrow = GLFW_KEY_LEFT,
    DownArrow = GLFW_KEY_DOWN,
    RightArrow = GLFW_KEY_RIGHT,
    
    Space = GLFW_KEY_SPACE,
    LeftShift = GLFW_KEY_LEFT_SHIFT,
    LeftControl = GLFW_KEY_LEFT_CONTROL,
    BackSpace = GLFW_KEY_BACKSPACE,
    Enter = GLFW_KEY_ENTER,
    
    NONE = GLFW_KEY_UNKNOWN
};

namespace Input
{
    extern GLFWwindow *window;

    void SetupInputCallbacks();
    void Update();

    bool IsKeyDown(KeyCode keyCode);
    int GetInputAxis(KeyCode negative, KeyCode positive);
    glm::vec2 GetInputVector2(KeyCode negativeX, KeyCode positiveX, KeyCode negativeY, KeyCode positiveY);

    Vector2 GetMousePosition();
    bool GetMouseButtonDown(int button);
    bool GetMouse0Down();
    bool GetMouse1Down();

    KeyCode GetPressedKey();
    Vector2 GetScrollDirection();

    KeyCode GetCurrentKey();
    std::string GetKeyName(KeyCode keyCode);
    std::string GetCurrentPrintableKey();
    int KeyCodeToGLFW(KeyCode keyCode);
}