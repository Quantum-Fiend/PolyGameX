#pragma once

// Placeholder for full ECS implementation

namespace PolyGameX {

class Component {
public:
    virtual ~Component() = default;
    virtual void Update(float deltaTime) = 0;
};

} // namespace PolyGameX
