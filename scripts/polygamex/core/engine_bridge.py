"""
Python-C++ bridge for integrating Python scripts with the engine.

This module provides ctypes bindings to the PolyGameX C++ engine,
allowing Python scripts to interact with the engine at runtime.
"""

import ctypes
import sys
import os
from pathlib import Path
from typing import Optional


class EngineBridge:
    """
    Bridge between Python and C++ engine.
    
    Provides access to engine functions and allows Python scripts
    to create entities, modify transforms, and interact with the scene.
    """
    
    def __init__(self, engine_dll_path: Optional[str] = None):
        """
        Initialize the engine bridge.
        
        Args:
            engine_dll_path: Path to PolyGameXCore.dll/.so
        """
        if engine_dll_path is None:
            # Auto-detect DLL path
            if sys.platform == "win32":
                engine_dll_path = "PolyGameXCore.dll"
            elif sys.platform == "darwin":
                engine_dll_path = "libPolyGameXCore.dylib"
            else:
                engine_dll_path = "libPolyGameXCore.so"
        
        try:
            self.engine = ctypes.CDLL(engine_dll_path)
            self._setup_function_signatures()
        except OSError as e:
            print(f"Failed to load engine library: {e}")
            print(f"Make sure {engine_dll_path} is in your PATH or current directory")
            self.engine = None
    
    def _setup_function_signatures(self):
        """Setup ctypes function signatures for engine functions."""
        if not self.engine:
            return
        
        # Example function signatures (would need to be exported from C++)
        # self.engine.CreateEntity.argtypes = [ctypes.c_char_p]
        # self.engine.CreateEntity.restype = ctypes.c_void_p
        
        # self.engine.SetEntityPosition.argtypes = [
        #     ctypes.c_void_p,  # entity
        #     ctypes.c_float,   # x
        #     ctypes.c_float,   # y
        #     ctypes.c_float    # z
        # ]
        # self.engine.SetEntityPosition.restype = None
        
        pass
    
    def is_loaded(self) -> bool:
        """Check if engine library is loaded."""
        return self.engine is not None
    
    def create_entity(self, name: str):
        """
        Create a new entity in the scene.
        
        Args:
            name: Entity name
            
        Returns:
            Entity handle (opaque pointer)
        """
        if not self.engine:
            print("Engine not loaded")
            return None
        
        # Example implementation (requires C++ exports)
        # return self.engine.CreateEntity(name.encode('utf-8'))
        
        print(f"Would create entity: {name}")
        return None
    
    def set_entity_position(self, entity, x: float, y: float, z: float):
        """
        Set entity position.
        
        Args:
            entity: Entity handle
            x, y, z: Position coordinates
        """
        if not self.engine or not entity:
            return
        
        # Example implementation
        # self.engine.SetEntityPosition(entity, x, y, z)
        
        print(f"Would set position: ({x}, {y}, {z})")


# Global bridge instance
_bridge: Optional[EngineBridge] = None


def get_engine_bridge() -> EngineBridge:
    """Get the global engine bridge instance."""
    global _bridge
    if _bridge is None:
        _bridge = EngineBridge()
    return _bridge


class GameObject:
    """
    Python wrapper for engine entities.
    
    Provides a Pythonic interface to engine objects.
    """
    
    def __init__(self, name: str):
        self.name = name
        self.bridge = get_engine_bridge()
        self._handle = self.bridge.create_entity(name)
    
    def set_position(self, x: float, y: float, z: float):
        """Set object position."""
        self.bridge.set_entity_position(self._handle, x, y, z)
    
    def __repr__(self):
        return f"GameObject('{self.name}')"


# Example usage
if __name__ == "__main__":
    print("PolyGameX Engine Bridge")
    print("=" * 40)
    
    bridge = get_engine_bridge()
    
    if bridge.is_loaded():
        print("✓ Engine loaded successfully")
        
        # Create a game object
        obj = GameObject("TestObject")
        obj.set_position(1.0, 2.0, 3.0)
    else:
        print("✗ Engine not loaded")
        print("\nTo use the engine bridge:")
        print("1. Build the C++ engine")
        print("2. Export C functions from the engine")
        print("3. Ensure the DLL/SO is in your PATH")
