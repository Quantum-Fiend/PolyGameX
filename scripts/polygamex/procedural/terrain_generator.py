"""
Procedural Terrain Generation using Perlin/Simplex Noise

This module generates realistic terrain heightmaps for use in the game engine.
"""

import numpy as np
from noise import pnoise2
from typing import Tuple, Optional


class TerrainGenerator:
    """
    Generates procedural terrain using Perlin noise.
    
    Features:
    - Multi-octave noise for realistic terrain
    - Configurable parameters (scale, octaves, persistence, lacunarity)
    - Export to various formats (heightmap, mesh data)
    """
    
    def __init__(self, width: int = 256, height: int = 256):
        """
        Initialize terrain generator.
        
        Args:
            width: Width of the terrain in vertices
            height: Height of the terrain in vertices
        """
        self.width = width
        self.height = height
        self.heightmap: Optional[np.ndarray] = None
        
    def generate(
        self,
        seed: int = 0,
        scale: float = 100.0,
        octaves: int = 6,
        persistence: float = 0.5,
        lacunarity: float = 2.0,
        height_multiplier: float = 50.0
    ) -> np.ndarray:
        """
        Generate terrain heightmap using Perlin noise.
        
        Args:
            seed: Random seed for reproducibility
            scale: Overall scale of the terrain features
            octaves: Number of noise layers (more = more detail)
            persistence: Amplitude decrease per octave
            lacunarity: Frequency increase per octave
            height_multiplier: Multiplier for final height values
            
        Returns:
            2D numpy array of height values
        """
        heightmap = np.zeros((self.height, self.width))
        
        for y in range(self.height):
            for x in range(self.width):
                # Normalize coordinates
                nx = x / self.width - 0.5
                ny = y / self.height - 0.5
                
                # Generate noise value
                noise_val = pnoise2(
                    nx * scale,
                    ny * scale,
                    octaves=octaves,
                    persistence=persistence,
                    lacunarity=lacunarity,
                    repeatx=self.width,
                    repeaty=self.height,
                    base=seed
                )
                
                # Normalize to [0, 1] and apply height multiplier
                heightmap[y][x] = (noise_val + 1.0) * 0.5 * height_multiplier
        
        self.heightmap = heightmap
        return heightmap
    
    def apply_erosion(self, iterations: int = 5, strength: float = 0.1):
        """
        Apply simple erosion simulation to make terrain more realistic.
        
        Args:
            iterations: Number of erosion passes
            strength: Strength of erosion effect
        """
        if self.heightmap is None:
            raise ValueError("Generate terrain first before applying erosion")
        
        for _ in range(iterations):
            eroded = self.heightmap.copy()
            
            for y in range(1, self.height - 1):
                for x in range(1, self.width - 1):
                    # Get neighboring heights
                    neighbors = [
                        self.heightmap[y-1][x], self.heightmap[y+1][x],
                        self.heightmap[y][x-1], self.heightmap[y][x+1]
                    ]
                    
                    # Erode if higher than neighbors
                    min_neighbor = min(neighbors)
                    if self.heightmap[y][x] > min_neighbor:
                        diff = (self.heightmap[y][x] - min_neighbor) * strength
                        eroded[y][x] -= diff
            
            self.heightmap = eroded
    
    def get_mesh_data(self) -> Tuple[np.ndarray, np.ndarray, np.ndarray]:
        """
        Convert heightmap to mesh data (vertices, normals, indices).
        
        Returns:
            Tuple of (vertices, normals, indices) as numpy arrays
        """
        if self.heightmap is None:
            raise ValueError("Generate terrain first before getting mesh data")
        
        vertices = []
        normals = []
        indices = []
        
        # Generate vertices and normals
        for y in range(self.height):
            for x in range(self.width):
                # Vertex position
                vx = x - self.width / 2.0
                vy = self.heightmap[y][x]
                vz = y - self.height / 2.0
                vertices.append([vx, vy, vz])
                
                # Calculate normal (simple method)
                if x > 0 and x < self.width - 1 and y > 0 and y < self.height - 1:
                    hL = self.heightmap[y][x-1]
                    hR = self.heightmap[y][x+1]
                    hD = self.heightmap[y-1][x]
                    hU = self.heightmap[y+1][x]
                    
                    nx = hL - hR
                    ny = 2.0
                    nz = hD - hU
                    
                    length = np.sqrt(nx*nx + ny*ny + nz*nz)
                    normals.append([nx/length, ny/length, nz/length])
                else:
                    normals.append([0.0, 1.0, 0.0])
        
        # Generate indices for triangles
        for y in range(self.height - 1):
            for x in range(self.width - 1):
                i0 = y * self.width + x
                i1 = i0 + 1
                i2 = i0 + self.width
                i3 = i2 + 1
                
                # Two triangles per quad
                indices.extend([i0, i2, i1, i1, i2, i3])
        
        return (
            np.array(vertices, dtype=np.float32),
            np.array(normals, dtype=np.float32),
            np.array(indices, dtype=np.uint32)
        )
    
    def save_heightmap(self, filename: str):
        """Save heightmap to file (numpy format)."""
        if self.heightmap is None:
            raise ValueError("Generate terrain first before saving")
        np.save(filename, self.heightmap)
    
    def load_heightmap(self, filename: str):
        """Load heightmap from file."""
        self.heightmap = np.load(filename)
        self.height, self.width = self.heightmap.shape


def generate_island_terrain(width: int = 256, height: int = 256, seed: int = 42) -> np.ndarray:
    """
    Generate island-shaped terrain with water around edges.
    
    Args:
        width: Terrain width
        height: Terrain height
        seed: Random seed
        
    Returns:
        Heightmap array
    """
    generator = TerrainGenerator(width, height)
    heightmap = generator.generate(seed=seed, scale=80.0, octaves=6)
    
    # Apply radial gradient to create island shape
    center_x, center_y = width / 2.0, height / 2.0
    max_dist = np.sqrt(center_x**2 + center_y**2)
    
    for y in range(height):
        for x in range(width):
            dx = x - center_x
            dy = y - center_y
            dist = np.sqrt(dx*dx + dy*dy)
            
            # Falloff from center
            falloff = 1.0 - (dist / max_dist) ** 2
            falloff = max(0.0, falloff)
            
            heightmap[y][x] *= falloff
    
    return heightmap


if __name__ == "__main__":
    # Demo usage
    print("Generating terrain...")
    terrain = TerrainGenerator(128, 128)
    heightmap = terrain.generate(seed=42, octaves=6, persistence=0.5)
    
    print(f"Generated terrain: {terrain.width}x{terrain.height}")
    print(f"Height range: {heightmap.min():.2f} to {heightmap.max():.2f}")
    
    terrain.apply_erosion(iterations=3)
    print("Applied erosion")
    
    vertices, normals, indices = terrain.get_mesh_data()
    print(f"Mesh data: {len(vertices)} vertices, {len(indices)//3} triangles")
