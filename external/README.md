# External Dependencies Setup

This directory contains third-party libraries required by PolyGameX.

## Required Libraries

### GLFW (Window Management)
Download from: https://www.glfw.org/
- Version: 3.3+
- Used for: Window creation, input handling, OpenGL context

### GLM (Mathematics)
Download from: https://github.com/g-truc/glm
- Version: 0.9.9+
- Used for: Vector/matrix math, transformations

### GLAD (OpenGL Loader)
Generate from: https://glad.dav1d.de/
- OpenGL Version: 4.3+
- Profile: Core
- Used for: Loading OpenGL functions

### stb_image (Image Loading)
Download from: https://github.com/nothings/stb
- Single header library
- Used for: Loading PNG, JPG, TGA textures

## Directory Structure

```
external/
├── glfw/
│   ├── include/
│   └── lib/
├── glm/
│   └── glm/
├── glad/
│   ├── include/
│   └── src/
└── stb/
    └── stb_image.h
```

## Setup Instructions

### Windows (Manual)
1. Download each library
2. Extract to respective folders
3. CMake will automatically find them

### Linux (Package Manager)
```bash
sudo apt-get install libglfw3-dev libglm-dev
# GLAD and stb_image need manual setup
```

### macOS (Homebrew)
```bash
brew install glfw glm
# GLAD and stb_image need manual setup
```

## Automated Setup (Recommended)

Run the setup script:
```bash
python scripts/setup_dependencies.py
```

This will automatically download and configure all dependencies.
