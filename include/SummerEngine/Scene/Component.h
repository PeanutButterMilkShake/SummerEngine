#pragma once

#include <vector>
#include <functional>
#include <string>

enum class PropertyType {
    Float,
    Int,
    String,
    Vector2,
    Vector3,
    Bool,
};

struct PropertyInfo {
    std::string name;
    PropertyType type;
    std::function<std::string()> get;
    std::function<void(const std::string&)> set;
};

class Object;

class Component
{
public:
    Component();
    ~Component();

    Object* object;

    virtual void Start();
    virtual void Update(float delta);
    virtual void SteppedUpdate(float delta);
    virtual void OnDestroy();

    virtual std::vector<PropertyInfo> GetProperties() { return {}; }

    template <typename T>
    T* GetComponent();

    bool hasStarted = false;
};

#define SPROPERTY(...)

#define SE_CAT_DIRECT(x, y) x##y
#define SE_CAT(x, y) SE_CAT_DIRECT(x, y)
#define SE_JOIN_3(a, b, c) SE_CAT(SE_CAT(a, b), c)
#define GENERATED_BODY_IMPL(file_id, line) SE_JOIN_3(file_id, _, line)
#define GENERATED_BODY() GENERATED_BODY_IMPL(CURRENT_FILE_ID, __LINE__)()