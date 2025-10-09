#include "Engine.h"
#include "FlyCamera.h"

int main()
{   
    Engine engine = Engine("Voxel Engine");

    FlyCamera flyCamera = FlyCamera();
    flyCamera.position = {-5, 0, 0};

    Camera camera = Camera();
    camera.parent = &flyCamera;

    Shader shader = Shader("D:\\GUSEProjects\\GUSEGraphicsEngine\\assets\\Shaders\\Vertex\\shader.vert", "D:\\GUSEProjects\\GUSEGraphicsEngine\\assets\\Shaders\\Fragment\\shader.frag");
    Mesh mesh = Mesh("D:\\GUSEProjects\\GUSEGraphicsEngine\\assets\\Models\\Cube.obj");
    mesh.shader = &shader;

    while (!glfwWindowShouldClose(engine.window))
    {
        glClearColor(0.01f, 0.00f, 0.03f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        engine.Update();

        glfwSwapBuffers(engine.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}