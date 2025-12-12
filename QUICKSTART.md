# Quick Start Guide

Get PolyGameX up and running in minutes!

## Prerequisites

Install the following tools:

- **CMake** 3.15+ - [Download](https://cmake.org/download/)
- **C++ Compiler**:
  - Windows: Visual Studio 2019+ with C++ tools
  - Linux: GCC 9+ (`sudo apt install build-essential`)
  - macOS: Xcode Command Line Tools (`xcode-select --install`)
- **Python** 3.8+ - [Download](https://www.python.org/downloads/)
- **Java** JDK 11+ - [Download](https://adoptium.net/)
- **Maven** - [Download](https://maven.apache.org/download.cgi)

## Step 1: Clone the Repository

```bash
git clone https://github.com/yourusername/PolyGameX.git
cd PolyGameX
```

## Step 2: Setup Dependencies

Run the automated dependency setup:

```bash
python scripts/setup_dependencies.py
```

This will guide you through setting up:
- GLFW (window management)
- GLM (mathematics)
- GLAD (OpenGL loader)
- stb_image (image loading)

> **Note**: Some dependencies may require manual download. Follow the on-screen instructions.

## Step 3: Build the Engine

### Windows

```batch
build.bat
```

### Linux/macOS

```bash
chmod +x build.sh
./build.sh
```

The build process will:
1. Configure the project with CMake
2. Compile the C++ engine
3. Create the demo executable

## Step 4: Install Python Modules

```bash
cd scripts
pip install -e .
cd ..
```

## Step 5: Build Java Modules (Optional)

```bash
cd platform
mvn clean package
cd ..
```

## Step 6: Run the Demo

### Windows

```batch
cd build\bin\Release
PolyGameXDemo.exe
```

### Linux/macOS

```bash
cd build/bin
./PolyGameXDemo
```

## What You'll See

The demo showcases:
- ✅ 5 cubes falling with realistic physics
- ✅ Collision detection and response
- ✅ Rotating sphere
- ✅ Orbiting camera
- ✅ Real-time FPS counter in console

Press **ESC** to exit.

## Next Steps

### Explore the Code

- **C++ Engine**: `src/` and `include/PolyGameX/`
- **Python AI**: `scripts/polygamex/`
- **Java Tools**: `platform/src/main/java/`
- **Demo**: `examples/demo/main.cpp`

### Try Python Modules

```python
# Terrain generation
from polygamex.procedural.terrain_generator import TerrainGenerator

terrain = TerrainGenerator(128, 128)
heightmap = terrain.generate(seed=42, octaves=6)
terrain.apply_erosion(iterations=3)

# Pathfinding
from polygamex.ai.pathfinding import Pathfinder

pathfinder = Pathfinder(50, 50)
path = pathfinder.find_path((5, 5), (45, 45))
```

### Read the Documentation

- [Architecture Guide](docs/ARCHITECTURE.md) - Technical deep dive
- [Walkthrough](../brain/736e28e9-7228-4f5f-97fb-73eebd6ebe89/walkthrough.md) - Complete feature overview
- [Contributing](CONTRIBUTING.md) - How to contribute

## Troubleshooting

### CMake not found
- Ensure CMake is installed and in your PATH
- Restart your terminal after installation

### Compiler errors
- Windows: Install Visual Studio with "Desktop development with C++"
- Linux: `sudo apt install build-essential cmake`
- macOS: `xcode-select --install`

### Missing dependencies
- Run `python scripts/setup_dependencies.py` again
- Check `external/README.md` for manual setup instructions

### Python module errors
- Ensure Python 3.8+ is installed
- Install dependencies: `pip install numpy noise scipy`

## Need Help?

- 📖 Check [README.md](README.md) for detailed information
- 🐛 Report issues on GitHub
- 💬 Ask questions in discussions

---

**Congratulations!** You're now ready to explore PolyGameX! 🎮
