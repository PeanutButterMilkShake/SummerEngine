#include "Engine.h"
#include "SteamTransport.h"
#include "Object.h"
#include "Light.h"
#include "Mesh.h"
#include "RectTransform.h"
#include "UIImage.h"

int main()
{   
    Engine engine = Engine("Summer Engine");

    Object playerObject;
    playerObject.AddComponent<Transform>();
    playerObject.AddComponent<Camera>();
    
    playerObject.GetComponent<Transform>()->position = Vector3(2,0,-5);

   // std::shared_ptr<Material> whiteMaterial  = ResourceManager::Get<Material>("WhiteMaterial", "assets/Shaders/Vertex/shader.vert", "assets/Shaders/Fragment/shader.frag");
   // whiteMaterial->SetProperty("baseColor", Color3(1));


    /*Mesh* playerMesh = playerObject.AddComponent<Mesh>();
    playerMesh->modelFilePath = "Assets/Models/monkey.obj";
    playerMesh->material = whiteMaterial;*/

    /*Object uiTest;
    uiTest.AddComponent<RectTransform>()->anchor = Vector2(-1,0);
    uiTest.GetComponent<RectTransform>()->pivot = Vector2(1,1);
    uiTest.AddComponent<UIImage>();*/

    Object sun;
    sun.AddComponent<Transform>();
    sun.AddComponent<Light>();
    sun.GetComponent<Transform>()->rotation = Quaternion::FromEuler(-72.0f, -360.0f, -108.0f);

    for(int i = 0; i < 5; i++)
    {
        Object* dragonObject = new Object();

        Transform* transform = dragonObject->AddComponent<Transform>();
        transform->position.x = i;
        transform->scale = Vector3(.01f);
        transform->rotation = Quaternion::FromEuler(0,90,0);

        std::shared_ptr<Material> newMat  = ResourceManager::Get<Material>(to_string(i), "assets/Shaders/Vertex/shader.vert", "assets/Shaders/Fragment/shader.frag");
        newMat->SetProperty("baseColor", Color3(.2 * i,0,1));

        Mesh* mesh = dragonObject->AddComponent<Mesh>();
        mesh->modelFilePath = "assets/Models/dragon.obj";
        mesh->material = newMat;
    }

    engine.ResetTime();

    while (!glfwWindowShouldClose(engine.window))
    {
        glClearColor(0.01f, 0.00f, 0.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //uiTest.GetComponent<RectTransform>()->position.y += sin(glfwGetTime()) * .01f;

        engine.Update();

        glfwSwapBuffers(engine.window);
        glfwPollEvents();

        if (Input::IsKeyDown(KeyCode::X))
            glfwSetWindowShouldClose(engine.window, 1);
    }

    glfwTerminate();
    return 0;
}