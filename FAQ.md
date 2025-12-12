# Frequently Asked Questions

## General Questions

### What is PolyGameX?
PolyGameX is a cross-platform game engine built with C++, Python, Java, and C#. It demonstrates advanced multi-language software engineering and provides a complete game development framework.

### Is it production-ready?
The core engine (C++ rendering, physics, Python AI) is fully functional and production-ready. The C# editor is planned but not yet implemented. You can build complete games with the current feature set.

### What license is it under?
MIT License - completely free for personal and commercial use.

### Can I use this for my game?
Absolutely! The MIT license allows you to use, modify, and distribute PolyGameX freely.

## Technical Questions

### What platforms are supported?
- ✅ Windows (primary)
- ✅ Linux (tested)
- ✅ macOS (compatible)

### What are the system requirements?
**Minimum**:
- OpenGL 4.3+ compatible GPU
- 4GB RAM
- C++17 compiler
- Python 3.8+
- Java 11+

**Recommended**:
- Modern GPU (2016+)
- 8GB RAM
- Visual Studio 2019+ / GCC 9+

### How do I integrate Python scripts?
```python
# 1. Install Python package
cd scripts
pip install -e .

# 2. Use in your game
from polygamex.ai.pathfinding import Pathfinder
pathfinder = Pathfinder(100, 100)
path = pathfinder.find_path((0, 0), (99, 99))
```

### Can I add my own shaders?
Yes! Create GLSL shaders and load them:
```cpp
Shader* myShader = new Shader();
myShader->CompileFromFile("vertex.glsl", "fragment.glsl");
renderer->DrawMesh(mesh, transform, myShader);
```

### How do I add custom components?
Extend the Component base class:
```cpp
class HealthComponent : public Component {
    float health = 100.0f;
    void Update(float deltaTime) override {
        // Update logic
    }
};
```

## Building & Setup

### Build fails with "CMake not found"
Install CMake 3.15+ and add to PATH:
- Windows: Download from cmake.org
- Linux: `sudo apt install cmake`
- macOS: `brew install cmake`

### Missing OpenGL headers
Install development libraries:
- Windows: Included with Visual Studio
- Linux: `sudo apt install libgl1-mesa-dev`
- macOS: Included with Xcode

### Python modules won't import
```bash
# Install in development mode
cd scripts
pip install -e .

# Or install dependencies manually
pip install numpy noise scipy
```

### Java build fails
Ensure Maven is installed:
```bash
# Check Maven
mvn --version

# Install if needed
# Windows: Download from maven.apache.org
# Linux: sudo apt install maven
# macOS: brew install maven
```

## Performance

### How many objects can I render?
Depends on complexity, but typically:
- Simple cubes: 10,000+
- Detailed meshes: 1,000+
- With physics: 500+

Use profiling to optimize:
```cpp
PROFILE_SCOPE("Rendering");
renderer->DrawScene();
```

### Physics is slow with many objects
Current implementation is O(n²). Optimize by:
1. Reducing active rigid bodies
2. Using static colliders for terrain
3. Implementing spatial partitioning (planned)

### How do I improve frame rate?
1. Profile with `Profiler::PrintResults()`
2. Reduce draw calls (batch meshes)
3. Optimize physics (fewer bodies)
4. Use LOD for distant objects (planned)

## Development

### Can I contribute?
Yes! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### How do I report bugs?
Open an issue on GitHub with:
- Steps to reproduce
- Expected vs actual behavior
- System information
- Relevant logs

### Where's the C# editor?
The foundation is laid, but the full WPF editor is not yet implemented. This is a planned feature for future releases.

### Can I add Lua scripting?
The architecture supports it! You would need to:
1. Integrate Lua library
2. Create bindings to C++ engine
3. Implement script loading system

### Will there be Vulkan support?
Planned for future releases. The renderer is designed to support multiple backends.

## Learning

### I'm new to game engines. Where do I start?
1. Read [GETTING_STARTED.md](GETTING_STARTED.md)
2. Run the demo: `examples/demo/main.cpp`
3. Modify the demo to experiment
4. Read [ARCHITECTURE.md](docs/ARCHITECTURE.md)

### How does the ECS work?
Entities are containers, Components hold data, Systems process components:
```cpp
Entity* player = scene->CreateEntity("Player");
player->SetMesh(mesh);  // Component
player->SetRigidBody(body);  // Component
// Systems update all components each frame
```

### Where can I learn about the physics?
Check `src/physics/PhysicsEngine.cpp` for implementation details. It uses:
- Verlet integration
- Impulse-based collision resolution
- Sphere-sphere collision detection

### How does Python integration work?
Via ctypes (C FFI):
```python
from ctypes import CDLL
engine = CDLL("PolyGameXCore.dll")
# Call C++ functions from Python
```

## Troubleshooting

### Demo won't run
1. Check build completed successfully
2. Ensure DLLs are in PATH (Windows)
3. Verify OpenGL drivers are up to date
4. Check console for error messages

### Black screen on startup
- Update graphics drivers
- Verify OpenGL 4.3+ support
- Check shader compilation logs
- Try running in compatibility mode

### Crashes on exit
Usually cleanup order issue. Ensure:
1. Delete entities before scene
2. Remove rigid bodies from physics
3. Call `engine.Shutdown()` last

## Still Have Questions?

- 📖 Check [Documentation](README.md)
- 💬 Open a GitHub Discussion
- 🐛 Report Issues on GitHub
- 📧 Contact maintainers

---

**Can't find your answer?** Open an issue and we'll add it to this FAQ!
