#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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

    bool IsKeyDown(KeyCode keyCode);
    int GetInputAxis(KeyCode negative, KeyCode positive);
    glm::vec2 GetInputVector2(KeyCode negativeX, KeyCode positiveX, KeyCode negativeY, KeyCode positiveY);

    int KeyCodeToGLFW(KeyCode keyCode);
}

#endif