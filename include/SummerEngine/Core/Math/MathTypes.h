#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#pragma once

struct Vector3
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;

    Vector3() = default;
    
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
    {

    }

    Vector3(float a) : x(a), y(a), z(a)
    {

    }

    Vector3 operator+(const Vector3& other) const {
        return Vector3{ x + other.x, y + other.y, z + other.z };
    }

    Vector3 operator-(const Vector3& other) const {
        return Vector3{ x - other.x, y - other.y, z - other.z };
    }

    Vector3 operator*(const Vector3& other) const {
        return Vector3{ x * other.x, y * other.y, z * other.z };
    }

    Vector3 operator/(const Vector3& other) const {
        return Vector3{ x / other.x, y / other.y, z / other.z };
    }

    Vector3 operator-() const
    {
        return Vector3(-x,-y, -z);
    }

    operator glm::vec3() const 
    {
        return glm::vec3(x, y, z);
    }

    Vector3(const glm::vec3& v) : x(v.x), y(v.y), z(v.z) {}

    bool operator==(const Vector3& other) const
    {
        return other.x == x && other.y == y && other.z == z;
    }
};

struct Vector2
{
    float x = 0.f;
    float y = 0.f;

    Vector2() = default;

    Vector2(float _x, float _y) : x(_x), y(_y) {}
    Vector2(float _s) : x(_s), y(_s) {}

    Vector2 operator+(const Vector2& other) const 
    {
        return Vector2{ x + other.x, y + other.y };
    }

    Vector2 operator-(const Vector2& other) const 
    {
        return Vector2{ x - other.x, y - other.y };
    }

    Vector2 operator*(const Vector2& other) const 
    {
        return Vector2{ x * other.x, y * other.y };
    }

    Vector2 operator/(const Vector2& other) const 
    {
        return Vector2{ x / other.x, y / other.y };
    }

    bool operator==(const Vector2& other) const
    {
        return other.x == x && other.y == y;
    }

    Vector2 operator-() const
    {
        return Vector2(-x,-y);
    }

    operator glm::vec2()
    {
        return glm::vec2(x,y);
    }
};

struct Quaternion
{
    float x = 0.0f, y = 0.0f, z = 0.0f;
    float w = 1.0f;

    Quaternion() = default;
    Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
    Quaternion(const glm::quat& q) : x(q.x), y(q.y), z(q.z), w(q.w) {}

    static Quaternion FromEuler(float pitchDeg, float yawDeg, float rollDeg)
    {
        // Convert Euler degrees directly using GLM (Pitch=X, Yaw=Y, Roll=Z)
        glm::quat q = glm::quat(glm::vec3(
            glm::radians(pitchDeg), 
            glm::radians(yawDeg), 
            glm::radians(rollDeg)
        ));
        return Quaternion(q);
    }

    static Quaternion FromAxisAngle(glm::vec3 axis, float angle)
    {
        float s = std::sin(angle / 2.0f);
        return Quaternion(axis.x * s, axis.y * s, axis.z * s, std::cos(angle / 2.0f));
    }

    Quaternion operator*(const Quaternion& b) const
    {
        return Quaternion(
            w * b.x + x * b.w + y * b.z - z * b.y,
            w * b.y - x * b.z + y * b.w + z * b.x,
            w * b.z + x * b.y - y * b.x + z * b.w,
            w * b.w - x * b.x - y * b.y - z * b.z
        );
    }

    Vector3 operator*(const Vector3& v) const 
    {
        return (glm::quat)(*this) * (glm::vec3)v;
    }

    void Normalize()
    {
        float length = std::sqrt(x * x + y * y + z * z + w * w);
        if (length > 0)
        {
            x /= length; y /= length; z /= length; w /= length;
        }
    }

    operator glm::quat() const 
    {
        return glm::quat(w, x, y, z);
    }
};

struct Color3
{
    float r = 0.f;
    float g = 0.f;
    float b = 0.f;

    Color3() = default;

    Color3(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}

    operator glm::vec3() const 
    {
        return glm::vec3(r, g, b);
    }

    operator Vector3() const 
    {
        return Vector3(r, g, b);
    }
};