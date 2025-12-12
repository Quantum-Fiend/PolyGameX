"""
Game script base class for user scripts.

Provides lifecycle hooks and engine access for game logic.
"""

from typing import Optional, Dict, Any


class GameScript:
    """
    Base class for all game scripts.
    
    Override lifecycle methods to implement custom game logic.
    
    Example:
        class PlayerController(GameScript):
            def on_start(self):
                self.speed = 5.0
            
            def on_update(self, delta_time):
                # Handle input and movement
                pass
    """
    
    def __init__(self, entity=None):
        """
        Initialize the script.
        
        Args:
            entity: The entity this script is attached to
        """
        self.entity = entity
        self.enabled = True
        self.context: Dict[str, Any] = {}
    
    def on_start(self):
        """
        Called when the script is first initialized.
        Use this for setup and initialization.
        """
        pass
    
    def on_update(self, delta_time: float):
        """
        Called every frame.
        
        Args:
            delta_time: Time since last frame in seconds
        """
        pass
    
    def on_fixed_update(self, fixed_delta_time: float):
        """
        Called at fixed time intervals (for physics).
        
        Args:
            fixed_delta_time: Fixed time step (usually 1/60)
        """
        pass
    
    def on_collision_enter(self, other):
        """
        Called when collision starts.
        
        Args:
            other: The other entity involved in collision
        """
        pass
    
    def on_collision_exit(self, other):
        """
        Called when collision ends.
        
        Args:
            other: The other entity involved in collision
        """
        pass
    
    def on_trigger_enter(self, other):
        """
        Called when entering a trigger volume.
        
        Args:
            other: The trigger entity
        """
        pass
    
    def on_trigger_exit(self, other):
        """
        Called when exiting a trigger volume.
        
        Args:
            other: The trigger entity
        """
        pass
    
    def on_destroy(self):
        """
        Called when the script is destroyed.
        Use this for cleanup.
        """
        pass
    
    def get_component(self, component_type):
        """
        Get a component from the entity.
        
        Args:
            component_type: Type of component to get
            
        Returns:
            Component instance or None
        """
        if self.entity:
            return self.entity.get_component(component_type)
        return None
    
    def __repr__(self):
        return f"{self.__class__.__name__}(entity={self.entity})"


class ExamplePlayerScript(GameScript):
    """Example player controller script."""
    
    def on_start(self):
        self.speed = 5.0
        self.jump_force = 10.0
        print(f"Player initialized with speed={self.speed}")
    
    def on_update(self, delta_time):
        # Example: Simple movement logic
        # In real implementation, would read input and move entity
        pass
    
    def on_collision_enter(self, other):
        print(f"Player collided with {other}")


class ExampleEnemyScript(GameScript):
    """Example enemy AI script."""
    
    def on_start(self):
        self.health = 100
        self.patrol_speed = 2.0
        self.chase_speed = 4.0
        self.state = "patrol"
    
    def on_update(self, delta_time):
        if self.state == "patrol":
            # Patrol logic
            pass
        elif self.state == "chase":
            # Chase player logic
            pass
    
    def take_damage(self, amount):
        self.health -= amount
        if self.health <= 0:
            self.on_death()
    
    def on_death(self):
        print("Enemy defeated!")
        # Trigger death animation, drop loot, etc.


if __name__ == "__main__":
    # Example usage
    print("Game Script System Demo")
    print("=" * 40)
    
    player = ExamplePlayerScript()
    player.on_start()
    player.on_update(0.016)  # 60 FPS
    
    enemy = ExampleEnemyScript()
    enemy.on_start()
    enemy.take_damage(50)
    print(f"Enemy health: {enemy.health}")
