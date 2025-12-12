"""
PolyGameX Python Scripting API

This module provides Python bindings for the PolyGameX engine,
allowing game logic, AI, and procedural generation to be written in Python.
"""

__version__ = "1.0.0"
__author__ = "PolyGameX Team"

from .procedural import terrain_generator, level_generator
from .ai import pathfinding, behavior_tree

__all__ = [
    'terrain_generator',
    'level_generator',
    'pathfinding',
    'behavior_tree',
]
