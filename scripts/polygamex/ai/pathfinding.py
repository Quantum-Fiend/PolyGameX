"""
A* Pathfinding Algorithm Implementation

Provides efficient pathfinding for AI navigation in the game world.
"""

import heapq
from typing import List, Tuple, Optional, Set
import numpy as np


class Node:
    """Represents a node in the pathfinding grid."""
    
    def __init__(self, x: int, y: int, walkable: bool = True):
        self.x = x
        self.y = y
        self.walkable = walkable
        self.g_cost = float('inf')  # Cost from start
        self.h_cost = 0.0  # Heuristic cost to end
        self.parent: Optional['Node'] = None
    
    @property
    def f_cost(self) -> float:
        """Total cost (g + h)."""
        return self.g_cost + self.h_cost
    
    def __lt__(self, other: 'Node') -> bool:
        return self.f_cost < other.f_cost
    
    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Node):
            return False
        return self.x == other.x and self.y == other.y
    
    def __hash__(self) -> int:
        return hash((self.x, self.y))


class Pathfinder:
    """
    A* pathfinding implementation for grid-based navigation.
    
    Features:
    - Efficient A* algorithm
    - Diagonal movement support
    - Obstacle avoidance
    - Path smoothing
    """
    
    def __init__(self, grid_width: int, grid_height: int):
        """
        Initialize pathfinder with grid dimensions.
        
        Args:
            grid_width: Width of the navigation grid
            grid_height: Height of the navigation grid
        """
        self.width = grid_width
        self.height = grid_height
        self.grid: List[List[Node]] = []
        
        # Create grid
        for y in range(grid_height):
            row = []
            for x in range(grid_width):
                row.append(Node(x, y))
            self.grid.append(row)
    
    def set_obstacle(self, x: int, y: int, is_obstacle: bool = True):
        """Mark a grid cell as obstacle or walkable."""
        if 0 <= x < self.width and 0 <= y < self.height:
            self.grid[y][x].walkable = not is_obstacle
    
    def set_obstacles_from_heightmap(self, heightmap: np.ndarray, max_slope: float = 0.5):
        """
        Set obstacles based on terrain heightmap slope.
        
        Args:
            heightmap: 2D array of height values
            max_slope: Maximum walkable slope
        """
        for y in range(1, self.height - 1):
            for x in range(1, self.width - 1):
                # Calculate slope
                h_center = heightmap[y][x]
                h_neighbors = [
                    heightmap[y-1][x], heightmap[y+1][x],
                    heightmap[y][x-1], heightmap[y][x+1]
                ]
                
                max_diff = max(abs(h_center - h) for h in h_neighbors)
                
                # Mark as obstacle if slope too steep
                if max_diff > max_slope:
                    self.grid[y][x].walkable = False
    
    def find_path(
        self,
        start: Tuple[int, int],
        end: Tuple[int, int],
        allow_diagonal: bool = True
    ) -> Optional[List[Tuple[int, int]]]:
        """
        Find path from start to end using A* algorithm.
        
        Args:
            start: Starting position (x, y)
            end: Target position (x, y)
            allow_diagonal: Whether to allow diagonal movement
            
        Returns:
            List of (x, y) positions forming the path, or None if no path found
        """
        start_x, start_y = start
        end_x, end_y = end
        
        # Validate positions
        if not self._is_valid_position(start_x, start_y) or \
           not self._is_valid_position(end_x, end_y):
            return None
        
        start_node = self.grid[start_y][start_x]
        end_node = self.grid[end_y][end_x]
        
        if not start_node.walkable or not end_node.walkable:
            return None
        
        # Reset grid
        for row in self.grid:
            for node in row:
                node.g_cost = float('inf')
                node.h_cost = 0.0
                node.parent = None
        
        # Initialize start node
        start_node.g_cost = 0
        start_node.h_cost = self._heuristic(start_node, end_node)
        
        # A* algorithm
        open_set = [start_node]
        closed_set: Set[Node] = set()
        
        while open_set:
            # Get node with lowest f_cost
            current = heapq.heappop(open_set)
            
            if current == end_node:
                return self._reconstruct_path(end_node)
            
            closed_set.add(current)
            
            # Check neighbors
            for neighbor in self._get_neighbors(current, allow_diagonal):
                if not neighbor.walkable or neighbor in closed_set:
                    continue
                
                # Calculate tentative g_cost
                movement_cost = 1.0 if (neighbor.x == current.x or neighbor.y == current.y) else 1.414
                tentative_g = current.g_cost + movement_cost
                
                if tentative_g < neighbor.g_cost:
                    neighbor.parent = current
                    neighbor.g_cost = tentative_g
                    neighbor.h_cost = self._heuristic(neighbor, end_node)
                    
                    if neighbor not in open_set:
                        heapq.heappush(open_set, neighbor)
        
        return None  # No path found
    
    def _is_valid_position(self, x: int, y: int) -> bool:
        """Check if position is within grid bounds."""
        return 0 <= x < self.width and 0 <= y < self.height
    
    def _heuristic(self, node_a: Node, node_b: Node) -> float:
        """Calculate heuristic distance (Euclidean)."""
        dx = abs(node_a.x - node_b.x)
        dy = abs(node_a.y - node_b.y)
        return np.sqrt(dx * dx + dy * dy)
    
    def _get_neighbors(self, node: Node, allow_diagonal: bool) -> List[Node]:
        """Get walkable neighbors of a node."""
        neighbors = []
        
        # Cardinal directions
        directions = [
            (0, -1), (0, 1), (-1, 0), (1, 0)  # Up, Down, Left, Right
        ]
        
        if allow_diagonal:
            directions.extend([
                (-1, -1), (-1, 1), (1, -1), (1, 1)  # Diagonals
            ])
        
        for dx, dy in directions:
            nx, ny = node.x + dx, node.y + dy
            if self._is_valid_position(nx, ny):
                neighbors.append(self.grid[ny][nx])
        
        return neighbors
    
    def _reconstruct_path(self, end_node: Node) -> List[Tuple[int, int]]:
        """Reconstruct path from end node to start."""
        path = []
        current = end_node
        
        while current is not None:
            path.append((current.x, current.y))
            current = current.parent
        
        path.reverse()
        return path
    
    def smooth_path(self, path: List[Tuple[int, int]]) -> List[Tuple[int, int]]:
        """
        Smooth path by removing unnecessary waypoints.
        
        Args:
            path: Original path
            
        Returns:
            Smoothed path
        """
        if len(path) <= 2:
            return path
        
        smoothed = [path[0]]
        current_index = 0
        
        while current_index < len(path) - 1:
            # Try to find furthest visible point
            for i in range(len(path) - 1, current_index, -1):
                if self._has_line_of_sight(path[current_index], path[i]):
                    smoothed.append(path[i])
                    current_index = i
                    break
        
        return smoothed
    
    def _has_line_of_sight(self, start: Tuple[int, int], end: Tuple[int, int]) -> bool:
        """Check if there's a clear line of sight between two points."""
        x0, y0 = start
        x1, y1 = end
        
        # Bresenham's line algorithm
        dx = abs(x1 - x0)
        dy = abs(y1 - y0)
        sx = 1 if x0 < x1 else -1
        sy = 1 if y0 < y1 else -1
        err = dx - dy
        
        while True:
            if not self._is_valid_position(x0, y0) or not self.grid[y0][x0].walkable:
                return False
            
            if x0 == x1 and y0 == y1:
                return True
            
            e2 = 2 * err
            if e2 > -dy:
                err -= dy
                x0 += sx
            if e2 < dx:
                err += dx
                y0 += sy


if __name__ == "__main__":
    # Demo usage
    print("Creating pathfinder...")
    pathfinder = Pathfinder(20, 20)
    
    # Add some obstacles
    for x in range(5, 15):
        pathfinder.set_obstacle(x, 10, True)
    
    print("Finding path...")
    path = pathfinder.find_path((2, 2), (18, 18), allow_diagonal=True)
    
    if path:
        print(f"Path found with {len(path)} waypoints")
        smoothed = pathfinder.smooth_path(path)
        print(f"Smoothed to {len(smoothed)} waypoints")
    else:
        print("No path found")
