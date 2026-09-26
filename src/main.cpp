#include "Engine.h"
#include "SteamTransport.h"
#include "Object.h"
#include "Light.h"
#include "Mesh.h"
#include "RectTransform.h"
#include "UIImage.h"
#include "UIButton.h"
#include "UIText.h"
#include "WorkspaceManager.h"
#include "EngineUI.h"

int main()
{   
    Engine engine = Engine("Summer Engine");

    Object* playerObject = new Object();
    playerObject->name = "Player";
    playerObject->AddComponent<Transform>()->position = Vector3(0, 0, -5);
    playerObject->AddComponent<Camera>();

    Object* playerMesh = new Object();
    playerMesh->name = "Mesh";
    playerMesh->SetParent(playerObject);

    Object* obj1 = new Object();
    obj1->name = "obj1";

    Object* obj2 = new Object();
    obj2->name = "obj2";

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

    delete playerObject;
    delete obj1;
    delete obj2;

    glfwTerminate();
    return 0;
}