#!/bin/bash

echo "========================================"
echo "PolyGameX Build Script (Linux/macOS)"
echo "========================================"
echo ""

# Check for CMake
if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake not found"
    echo "Please install CMake 3.15 or higher"
    exit 1
fi

# Create build directory
mkdir -p build
cd build

echo "Configuring project with CMake..."
cmake ..
if [ $? -ne 0 ]; then
    echo "ERROR: CMake configuration failed"
    cd ..
    exit 1
fi

echo ""
echo "Building project..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu)
if [ $? -ne 0 ]; then
    echo "ERROR: Build failed"
    cd ..
    exit 1
fi

cd ..

echo ""
echo "========================================"
echo "Build completed successfully!"
echo "========================================"
echo ""
echo "Binaries located in: build/bin"
echo ""
echo "To run the demo:"
echo "  cd build/bin"
echo "  ./PolyGameXDemo"
echo ""
