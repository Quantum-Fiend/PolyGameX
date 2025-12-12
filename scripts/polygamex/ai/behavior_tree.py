"""
Behavior Tree System for AI

Provides a flexible behavior tree implementation for complex NPC AI.
"""

from enum import Enum
from typing import List, Callable, Optional
from abc import ABC, abstractmethod


class NodeStatus(Enum):
    """Status returned by behavior tree nodes."""
    SUCCESS = 1
    FAILURE = 2
    RUNNING = 3


class BehaviorNode(ABC):
    """Base class for all behavior tree nodes."""
    
    def __init__(self, name: str = "Node"):
        self.name = name
        self.status = NodeStatus.FAILURE
    
    @abstractmethod
    def tick(self, context: dict) -> NodeStatus:
        """
        Execute the node's behavior.
        
        Args:
            context: Shared context dictionary for the behavior tree
            
        Returns:
            NodeStatus indicating the result
        """
        pass
    
    def reset(self):
        """Reset the node to its initial state."""
        self.status = NodeStatus.FAILURE


class ActionNode(BehaviorNode):
    """Leaf node that performs an action."""
    
    def __init__(self, name: str, action: Callable[[dict], NodeStatus]):
        super().__init__(name)
        self.action = action
    
    def tick(self, context: dict) -> NodeStatus:
        self.status = self.action(context)
        return self.status


class ConditionNode(BehaviorNode):
    """Leaf node that checks a condition."""
    
    def __init__(self, name: str, condition: Callable[[dict], bool]):
        super().__init__(name)
        self.condition = condition
    
    def tick(self, context: dict) -> NodeStatus:
        self.status = NodeStatus.SUCCESS if self.condition(context) else NodeStatus.FAILURE
        return self.status


class CompositeNode(BehaviorNode):
    """Base class for nodes with multiple children."""
    
    def __init__(self, name: str, children: Optional[List[BehaviorNode]] = None):
        super().__init__(name)
        self.children = children or []
    
    def add_child(self, child: BehaviorNode):
        """Add a child node."""
        self.children.append(child)
    
    def reset(self):
        super().reset()
        for child in self.children:
            child.reset()


class SequenceNode(CompositeNode):
    """
    Executes children in order until one fails.
    Returns SUCCESS if all children succeed.
    """
    
    def __init__(self, name: str = "Sequence", children: Optional[List[BehaviorNode]] = None):
        super().__init__(name, children)
        self.current_child = 0
    
    def tick(self, context: dict) -> NodeStatus:
        while self.current_child < len(self.children):
            status = self.children[self.current_child].tick(context)
            
            if status == NodeStatus.RUNNING:
                self.status = NodeStatus.RUNNING
                return self.status
            elif status == NodeStatus.FAILURE:
                self.current_child = 0
                self.status = NodeStatus.FAILURE
                return self.status
            
            self.current_child += 1
        
        self.current_child = 0
        self.status = NodeStatus.SUCCESS
        return self.status
    
    def reset(self):
        super().reset()
        self.current_child = 0


class SelectorNode(CompositeNode):
    """
    Executes children in order until one succeeds.
    Returns FAILURE if all children fail.
    """
    
    def __init__(self, name: str = "Selector", children: Optional[List[BehaviorNode]] = None):
        super().__init__(name, children)
        self.current_child = 0
    
    def tick(self, context: dict) -> NodeStatus:
        while self.current_child < len(self.children):
            status = self.children[self.current_child].tick(context)
            
            if status == NodeStatus.RUNNING:
                self.status = NodeStatus.RUNNING
                return self.status
            elif status == NodeStatus.SUCCESS:
                self.current_child = 0
                self.status = NodeStatus.SUCCESS
                return self.status
            
            self.current_child += 1
        
        self.current_child = 0
        self.status = NodeStatus.FAILURE
        return self.status
    
    def reset(self):
        super().reset()
        self.current_child = 0


class ParallelNode(CompositeNode):
    """
    Executes all children simultaneously.
    Success/failure policy can be configured.
    """
    
    def __init__(
        self,
        name: str = "Parallel",
        children: Optional[List[BehaviorNode]] = None,
        success_threshold: int = 1
    ):
        super().__init__(name, children)
        self.success_threshold = success_threshold
    
    def tick(self, context: dict) -> NodeStatus:
        success_count = 0
        failure_count = 0
        
        for child in self.children:
            status = child.tick(context)
            
            if status == NodeStatus.SUCCESS:
                success_count += 1
            elif status == NodeStatus.FAILURE:
                failure_count += 1
        
        if success_count >= self.success_threshold:
            self.status = NodeStatus.SUCCESS
        elif failure_count > len(self.children) - self.success_threshold:
            self.status = NodeStatus.FAILURE
        else:
            self.status = NodeStatus.RUNNING
        
        return self.status


class DecoratorNode(BehaviorNode):
    """Base class for decorator nodes that modify child behavior."""
    
    def __init__(self, name: str, child: BehaviorNode):
        super().__init__(name)
        self.child = child
    
    def reset(self):
        super().reset()
        self.child.reset()


class InverterNode(DecoratorNode):
    """Inverts the result of its child."""
    
    def tick(self, context: dict) -> NodeStatus:
        status = self.child.tick(context)
        
        if status == NodeStatus.SUCCESS:
            self.status = NodeStatus.FAILURE
        elif status == NodeStatus.FAILURE:
            self.status = NodeStatus.SUCCESS
        else:
            self.status = NodeStatus.RUNNING
        
        return self.status


class RepeatNode(DecoratorNode):
    """Repeats its child a specified number of times."""
    
    def __init__(self, name: str, child: BehaviorNode, count: int = -1):
        super().__init__(name, child)
        self.count = count  # -1 for infinite
        self.current_count = 0
    
    def tick(self, context: dict) -> NodeStatus:
        if self.count > 0 and self.current_count >= self.count:
            self.current_count = 0
            self.status = NodeStatus.SUCCESS
            return self.status
        
        status = self.child.tick(context)
        
        if status != NodeStatus.RUNNING:
            self.current_count += 1
            self.child.reset()
        
        self.status = NodeStatus.RUNNING if self.count == -1 or self.current_count < self.count else NodeStatus.SUCCESS
        return self.status
    
    def reset(self):
        super().reset()
        self.current_count = 0


class BehaviorTree:
    """
    Main behavior tree class.
    
    Example usage:
        tree = BehaviorTree("EnemyAI")
        tree.root = SelectorNode("Root", [
            SequenceNode("Attack", [
                ConditionNode("PlayerInRange", lambda ctx: ctx['player_distance'] < 5),
                ActionNode("AttackPlayer", attack_action)
            ]),
            ActionNode("Patrol", patrol_action)
        ])
    """
    
    def __init__(self, name: str):
        self.name = name
        self.root: Optional[BehaviorNode] = None
        self.context: dict = {}
    
    def tick(self) -> NodeStatus:
        """Execute one tick of the behavior tree."""
        if self.root is None:
            return NodeStatus.FAILURE
        return self.root.tick(self.context)
    
    def reset(self):
        """Reset the entire tree."""
        if self.root:
            self.root.reset()
        self.context.clear()


if __name__ == "__main__":
    # Demo: Simple enemy AI
    print("Creating behavior tree...")
    
    def check_player_nearby(ctx):
        return ctx.get('player_distance', 100) < 10
    
    def attack_player(ctx):
        print("Attacking player!")
        return NodeStatus.SUCCESS
    
    def patrol(ctx):
        print("Patrolling...")
        return NodeStatus.SUCCESS
    
    tree = BehaviorTree("EnemyAI")
    tree.root = SelectorNode("Root", [
        SequenceNode("AttackSequence", [
            ConditionNode("PlayerNearby", check_player_nearby),
            ActionNode("Attack", attack_player)
        ]),
        ActionNode("Patrol", patrol)
    ])
    
    # Test with player far away
    tree.context['player_distance'] = 50
    print("\nPlayer far away:")
    tree.tick()
    
    # Test with player nearby
    tree.context['player_distance'] = 5
    print("\nPlayer nearby:")
    tree.tick()
