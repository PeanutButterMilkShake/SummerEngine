#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "MathTypes.h"

enum KeyCode
{
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,

    UpArrow,
    LeftArrow,
    DownArrow,
    RightArrow,

    Space,
    LeftShift,
    LeftControl,
};

namespace Input
{
    extern GLFWwindow *window;

    // Keyboard input
    bool IsKeyDown(KeyCode keyCode);
    int GetInputAxis(KeyCode negative, KeyCode positive);
    glm::vec2 GetInputVector2(KeyCode negativeX, KeyCode positiveX, KeyCode negativeY, KeyCode positiveY);

    // Mouse input
    Vector2 GetMousePosition();
    bool GetMouseButtonDown(int button);
    bool GetMouse0Down();
    bool GetMouse1Down();

    int KeyCodeToGLFW(KeyCode keyCode);
}