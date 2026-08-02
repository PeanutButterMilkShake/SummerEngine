#include "Engine.h"
#include "SteamTransport.h"
#include "Object.h"
#include "Light.h"
#include "Mesh.h"
#include "RectTransform.h"
#include "UIImage.h"
#include "UIButton.h"

int main()
{   
    Engine engine = Engine("Summer Engine");

    Object playerObject;
    playerObject.AddComponent<Transform>()->position = Vector3(0,0,-5);
    playerObject.AddComponent<Camera>();

    Object sun;
    sun.AddComponent<Transform>();
    sun.AddComponent<Light>();
    sun.GetComponent<Transform>()->rotation = Quaternion::FromEuler(100, 0, 0);


    engine.ResetTime();

    while (!glfwWindowShouldClose(engine.window))
    {
        glClearColor(0.01f, 0.00f, 0.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        engine.Update();

        glfwSwapBuffers(engine.window);
        glfwPollEvents();

        if (Input::IsKeyDown(KeyCode::X))
            glfwSetWindowShouldClose(engine.window, 1);
    }

    glfwTerminate();
    return 0;
}