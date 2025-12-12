"""
Dependency setup script for PolyGameX.

Automatically downloads and configures external dependencies.
"""

import os
import sys
import urllib.request
import zipfile
import shutil
from pathlib import Path


class DependencySetup:
    def __init__(self):
        self.project_root = Path(__file__).parent.parent
        self.external_dir = self.project_root / "external"
        self.external_dir.mkdir(exist_ok=True)
    
    def download_file(self, url, destination):
        """Download a file from URL to destination."""
        print(f"Downloading {url}...")
        try:
            urllib.request.urlretrieve(url, destination)
            print(f"Downloaded to {destination}")
            return True
        except Exception as e:
            print(f"Error downloading: {e}")
            return False
    
    def extract_zip(self, zip_path, extract_to):
        """Extract a ZIP file."""
        print(f"Extracting {zip_path}...")
        try:
            with zipfile.ZipFile(zip_path, 'r') as zip_ref:
                zip_ref.extractall(extract_to)
            print(f"Extracted to {extract_to}")
            return True
        except Exception as e:
            print(f"Error extracting: {e}")
            return False
    
    def setup_glad(self):
        """Setup GLAD OpenGL loader."""
        print("\n=== Setting up GLAD ===")
        glad_dir = self.external_dir / "glad"
        
        if glad_dir.exists():
            print("GLAD already exists, skipping...")
            return True
        
        print("Please generate GLAD from https://glad.dav1d.de/")
        print("Settings:")
        print("  - Language: C/C++")
        print("  - Specification: OpenGL")
        print("  - API gl: Version 4.3+")
        print("  - Profile: Core")
        print("  - Generate a loader: YES")
        print("\nDownload the ZIP and extract to: external/glad/")
        print("Then re-run this script.")
        return False
    
    def setup_glm(self):
        """Setup GLM mathematics library."""
        print("\n=== Setting up GLM ===")
        glm_dir = self.external_dir / "glm"
        
        if glm_dir.exists():
            print("GLM already exists, skipping...")
            return True
        
        # GLM is header-only, can be cloned or downloaded
        print("GLM setup required.")
        print("Option 1: Clone from GitHub:")
        print("  git clone https://github.com/g-truc/glm.git external/glm")
        print("\nOption 2: Download release from:")
        print("  https://github.com/g-truc/glm/releases")
        print("  Extract to: external/glm/")
        return False
    
    def setup_stb(self):
        """Setup stb_image library."""
        print("\n=== Setting up stb_image ===")
        stb_dir = self.external_dir / "stb"
        stb_dir.mkdir(exist_ok=True)
        
        stb_image = stb_dir / "stb_image.h"
        if stb_image.exists():
            print("stb_image.h already exists, skipping...")
            return True
        
        url = "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h"
        return self.download_file(url, stb_image)
    
    def setup_glfw(self):
        """Setup GLFW library."""
        print("\n=== Setting up GLFW ===")
        glfw_dir = self.external_dir / "glfw"
        
        if glfw_dir.exists():
            print("GLFW already exists, skipping...")
            return True
        
        print("GLFW setup required.")
        print("Option 1: Use package manager (recommended):")
        if sys.platform == "win32":
            print("  Download from: https://www.glfw.org/download.html")
            print("  Extract to: external/glfw/")
        elif sys.platform == "linux":
            print("  sudo apt-get install libglfw3-dev")
        elif sys.platform == "darwin":
            print("  brew install glfw")
        
        print("\nOption 2: Build from source:")
        print("  git clone https://github.com/glfw/glfw.git external/glfw")
        print("  cd external/glfw")
        print("  cmake . && make")
        return False
    
    def verify_setup(self):
        """Verify all dependencies are set up."""
        print("\n=== Verifying Dependencies ===")
        
        required = {
            "GLAD": self.external_dir / "glad" / "include" / "glad" / "glad.h",
            "GLM": self.external_dir / "glm" / "glm" / "glm.hpp",
            "stb_image": self.external_dir / "stb" / "stb_image.h",
        }
        
        all_ok = True
        for name, path in required.items():
            if path.exists():
                print(f"✓ {name}: OK")
            else:
                print(f"✗ {name}: MISSING ({path})")
                all_ok = False
        
        return all_ok
    
    def run(self):
        """Run the setup process."""
        print("=" * 60)
        print("PolyGameX Dependency Setup")
        print("=" * 60)
        
        # Setup each dependency
        self.setup_glad()
        self.setup_glm()
        self.setup_stb()
        self.setup_glfw()
        
        # Verify
        if self.verify_setup():
            print("\n✓ All dependencies set up successfully!")
            print("\nYou can now build the project:")
            print("  Windows: build.bat")
            print("  Linux/macOS: ./build.sh")
        else:
            print("\n✗ Some dependencies are missing.")
            print("Please follow the instructions above to complete setup.")
            return 1
        
        return 0


if __name__ == "__main__":
    setup = DependencySetup()
    sys.exit(setup.run())
