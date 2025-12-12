# Getting Started with PolyGameX

Welcome to PolyGameX! This guide will help you create your first game using the engine.

## Your First Game in 5 Minutes

### Step 1: Create a New Scene

```cpp
#include <PolyGameX/Engine.h>
#include <PolyGameX/Scene/SceneManager.h>
#include <PolyGameX/ECS/Entity.h>
#include <PolyGameX/Rendering/Mesh.h>
#include <PolyGameX/Rendering/Camera.h>

using namespace PolyGameX;

int main() {
    // Initialize engine
    Engine& engine = Engine::GetInstance();
    engine.Initialize(1280, 720, "My First Game");
    
    // Create camera
    Camera* camera = new Camera(CameraType::Perspective);
    camera->SetPosition(glm::vec3(0, 2, 5));
    camera->SetPerspective(45.0f, 16.0f/9.0f, 0.1f, 100.0f);
    engine.GetRenderer()->SetActiveCamera(camera);
    
    // Create scene
    Scene* scene = engine.GetSceneManager()->CreateScene("MainScene");
    engine.GetSceneManager()->SetActiveScene(scene);
    
    // Create a spinning cube
    Entity* cube = scene->CreateEntity("MyCube");
    cube->SetMesh(Mesh::CreateCube());
    cube->SetPosition(glm::vec3(0, 0, 0));
    
    // Game loop
    while (engine.IsRunning()) {
        float time = engine.GetTime();
        cube->SetRotation(glm::vec3(time * 30, time * 45, 0));
        engine.Run();
    }
    
    // Cleanup
    delete camera;
    engine.Shutdown();
    return 0;
}
```

### Step 2: Add Physics

```cpp
#include <PolyGameX/Physics/RigidBody.h>
#include <PolyGameX/Physics/Collider.h>

// Create a falling sphere
Entity* sphere = scene->CreateEntity("PhysicsSphere");
sphere->SetMesh(Mesh::CreateSphere(32));
sphere->SetPosition(glm::vec3(0, 5, 0));

// Add physics
RigidBody* body = new RigidBody();
body->SetPosition(sphere->GetPosition());
body->SetMass(1.0f);

Collider* collider = new Collider(ColliderType::Sphere);
collider->SetRadius(0.5f);
body->SetCollider(collider);

sphere->SetRigidBody(body);
engine.GetPhysics()->AddRigidBody(body);
```

### Step 3: Add AI with Python

Create `enemy_ai.py`:

```python
from polygamex.ai.behavior_tree import BehaviorTree, SelectorNode, SequenceNode
from polygamex.ai.behavior_tree import ConditionNode, ActionNode

class EnemyAI:
    def __init__(self):
        self.tree = BehaviorTree("Enemy")
        self.tree.root = SelectorNode("Root", [
            SequenceNode("Attack", [
                ConditionNode("PlayerClose", self.is_player_close),
                ActionNode("DoAttack", self.attack)
            ]),
            ActionNode("Patrol", self.patrol)
        ])
    
    def is_player_close(self, ctx):
        return ctx.get('player_distance', 100) < 10
    
    def attack(self, ctx):
        print("Attacking player!")
        return NodeStatus.SUCCESS
    
    def patrol(self, ctx):
        print("Patrolling...")
        return NodeStatus.SUCCESS
    
    def update(self):
        self.tree.tick()
```

### Step 4: Generate Terrain

```python
from polygamex.procedural.terrain_generator import TerrainGenerator

# Generate terrain
terrain = TerrainGenerator(256, 256)
heightmap = terrain.generate(seed=42, octaves=6, persistence=0.5)
terrain.apply_erosion(iterations=3)

# Export mesh data
vertices, normals, indices = terrain.get_mesh_data()

# Save for use in engine
terrain.save_heightmap("terrain.npy")
```

## Common Patterns

### Creating Game Objects

```cpp
// Template for game objects
Entity* CreatePlayer(Scene* scene) {
    Entity* player = scene->CreateEntity("Player");
    player->SetMesh(Mesh::CreateCube());
    player->SetPosition(glm::vec3(0, 1, 0));
    player->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
    
    // Add physics
    RigidBody* body = new RigidBody();
    body->SetPosition(player->GetPosition());
    body->SetMass(70.0f); // kg
    
    Collider* collider = new Collider(ColliderType::Box);
    collider->SetExtents(glm::vec3(0.5f, 1.0f, 0.5f));
    body->SetCollider(collider);
    
    player->SetRigidBody(body);
    return player;
}
```

### Camera Controls

```cpp
void UpdateCamera(Camera* camera, float deltaTime) {
    // Orbit around origin
    static float angle = 0.0f;
    angle += deltaTime * 0.5f;
    
    float radius = 10.0f;
    float x = cos(angle) * radius;
    float z = sin(angle) * radius;
    
    camera->SetPosition(glm::vec3(x, 5, z));
    camera->LookAt(glm::vec3(0, 0, 0));
}
```

### Performance Profiling

```cpp
#include <PolyGameX/Utils/Profiler.h>

void ExpensiveFunction() {
    PROFILE_FUNCTION();
    // Your code here
}

void GameLoop() {
    PROFILE_SCOPE("GameLoop");
    
    {
        PROFILE_SCOPE("Physics");
        engine.GetPhysics()->Update(deltaTime);
    }
    
    {
        PROFILE_SCOPE("Rendering");
        engine.GetRenderer()->Render();
    }
}

// Print results
Profiler::GetInstance().PrintResults();
```

## Next Steps

1. **Explore Examples**: Check `examples/demo/main.cpp`
2. **Read Documentation**: See `docs/ARCHITECTURE.md`
3. **Experiment**: Modify the demo and create your own scenes
4. **Add Features**: Extend with custom components and systems

## Troubleshooting

**Q: Engine won't build**
- Ensure all dependencies are installed
- Run `python scripts/setup_dependencies.py`
- Check CMake output for missing libraries

**Q: Physics not working**
- Verify rigid bodies are added to physics engine
- Check collider sizes and positions
- Enable debug drawing to visualize

**Q: Python scripts not running**
- Install Python package: `pip install -e scripts/`
- Check Python version (3.8+ required)
- Verify engine DLL is accessible

## Resources

- [Full Documentation](README.md)
- [Architecture Guide](docs/ARCHITECTURE.md)
- [API Reference](docs/API.md) (planned)
- [Community Examples](examples/)

Happy game development! 🎮
