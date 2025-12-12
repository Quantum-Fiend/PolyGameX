#include <PolyGameX/Engine.h>
#include <PolyGameX/Rendering/Mesh.h>
#include <PolyGameX/Rendering/Camera.h>
#include <PolyGameX/Scene/SceneManager.h>
#include <PolyGameX/ECS/Entity.h>
#include <PolyGameX/Physics/RigidBody.h>
#include <PolyGameX/Physics/Collider.h>
#include <PolyGameX/Core/Logger.h>
#include <iostream>

using namespace PolyGameX;

int main() {
    Logger::Info("=== PolyGameX Demo Application ===");
    Logger::Info("Multi-Language Game Engine Demonstration");
    Logger::Info("");
    
    // Initialize engine
    Engine& engine = Engine::GetInstance();
    if (!engine.Initialize(1280, 720, "PolyGameX Demo - Multi-Language Game Engine")) {
        Logger::Error("Failed to initialize engine!");
        return -1;
    }
    
    // Create camera
    Camera* camera = new Camera(CameraType::Perspective);
    camera->SetPosition(glm::vec3(0.0f, 2.0f, 5.0f));
    camera->SetPerspective(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
    camera->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    engine.GetRenderer()->SetActiveCamera(camera);
    
    // Create scene
    Scene* scene = engine.GetSceneManager()->CreateScene("DemoScene");
    engine.GetSceneManager()->SetActiveScene(scene);
    
    // Create meshes
    Mesh* cubeMesh = Mesh::CreateCube();
    Mesh* sphereMesh = Mesh::CreateSphere(32);
    Mesh* planeMesh = Mesh::CreatePlane(10.0f, 10.0f);
    
    // Create ground plane
    Entity* ground = scene->CreateEntity("Ground");
    ground->SetMesh(planeMesh);
    ground->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    ground->SetScale(glm::vec3(2.0f, 1.0f, 2.0f));
    
    // Create static ground rigid body
    RigidBody* groundBody = new RigidBody();
    groundBody->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    groundBody->SetStatic(true);
    Collider* groundCollider = new Collider(ColliderType::Box);
    groundCollider->SetExtents(glm::vec3(10.0f, 0.1f, 10.0f));
    groundBody->SetCollider(groundCollider);
    ground->SetRigidBody(groundBody);
    engine.GetPhysics()->AddRigidBody(groundBody);
    
    // Create falling cubes with physics
    std::vector<Entity*> cubes;
    std::vector<RigidBody*> cubeRigidBodies;
    
    for (int i = 0; i < 5; ++i) {
        Entity* cube = scene->CreateEntity("Cube_" + std::to_string(i));
        cube->SetMesh(cubeMesh);
        cube->SetPosition(glm::vec3(-2.0f + i * 1.0f, 3.0f + i * 1.5f, 0.0f));
        cube->SetScale(glm::vec3(0.5f));
        
        RigidBody* rigidBody = new RigidBody();
        rigidBody->SetPosition(cube->GetPosition());
        rigidBody->SetMass(1.0f);
        
        Collider* collider = new Collider(ColliderType::Sphere);
        collider->SetRadius(0.5f);
        rigidBody->SetCollider(collider);
        
        cube->SetRigidBody(rigidBody);
        engine.GetPhysics()->AddRigidBody(rigidBody);
        
        cubes.push_back(cube);
        cubeRigidBodies.push_back(rigidBody);
    }
    
    // Create rotating sphere
    Entity* sphere = scene->CreateEntity("Sphere");
    sphere->SetMesh(sphereMesh);
    sphere->SetPosition(glm::vec3(3.0f, 1.0f, 0.0f));
    sphere->SetScale(glm::vec3(0.8f));
    
    Logger::Info("Scene created with:");
    Logger::Info("  - Ground plane");
    Logger::Info("  - 5 falling cubes with physics");
    Logger::Info("  - 1 rotating sphere");
    Logger::Info("");
    Logger::Info("Starting main loop...");
    Logger::Info("Press ESC to exit");
    Logger::Info("");
    
    float time = 0.0f;
    
    // Main loop
    while (engine.IsRunning()) {
        time += engine.GetDeltaTime();
        
        // Rotate sphere
        sphere->SetRotation(glm::vec3(
            time * 30.0f,
            time * 45.0f,
            time * 60.0f
        ));
        
        // Rotate camera around scene
        float radius = 8.0f;
        float camX = std::sin(time * 0.3f) * radius;
        float camZ = std::cos(time * 0.3f) * radius;
        camera->SetPosition(glm::vec3(camX, 3.0f, camZ));
        camera->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        
        // Run engine
        engine.Run();
        
        // Print stats every 60 frames
        static int frameCount = 0;
        if (++frameCount % 60 == 0) {
            auto stats = engine.GetRenderer()->GetStats();
            Logger::Debug("FPS: " + std::to_string(1.0f / engine.GetDeltaTime()) + 
                         " | Draw Calls: " + std::to_string(stats.drawCalls) +
                         " | Triangles: " + std::to_string(stats.triangles));
        }
    }
    
    // Cleanup
    Logger::Info("Cleaning up...");
    
    for (auto* body : cubeRigidBodies) {
        delete body->GetCollider();
        delete body;
    }
    
    delete groundBody->GetCollider();
    delete groundBody;
    
    delete cubeMesh;
    delete sphereMesh;
    delete planeMesh;
    delete camera;
    
    engine.Shutdown();
    
    Logger::Info("Demo completed successfully!");
    return 0;
}
