# Contributing to PolyGameX

Thank you for your interest in contributing to PolyGameX! This document provides guidelines for contributing to the project.

## Code of Conduct

- Be respectful and inclusive
- Focus on constructive feedback
- Help others learn and grow

## How to Contribute

### Reporting Bugs

1. Check if the bug has already been reported
2. Create a detailed issue with:
   - Steps to reproduce
   - Expected vs actual behavior
   - System information (OS, compiler, etc.)
   - Relevant code snippets or logs

### Suggesting Features

1. Open an issue describing the feature
2. Explain the use case and benefits
3. Discuss implementation approach

### Pull Requests

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Write/update tests if applicable
5. Update documentation
6. Commit with clear messages
7. Push to your fork
8. Open a Pull Request

## Coding Standards

### C/C++

- Follow C++17 standard
- Use RAII for resource management
- Prefer smart pointers over raw pointers
- Use const correctness
- Format with clang-format (provided config)

```cpp
// Good
auto mesh = std::make_unique<Mesh>();
const glm::vec3& position = entity->GetPosition();

// Avoid
Mesh* mesh = new Mesh();
glm::vec3 position = entity->GetPosition(); // unnecessary copy
```

### Python

- Follow PEP 8 style guide
- Use type hints where appropriate
- Write docstrings for public APIs
- Format with black

```python
def find_path(
    self,
    start: Tuple[int, int],
    end: Tuple[int, int]
) -> Optional[List[Tuple[int, int]]]:
    """Find path from start to end using A*."""
    pass
```

### Java

- Follow Google Java Style Guide
- Use meaningful variable names
- Write Javadoc for public methods
- Handle exceptions appropriately

```java
/**
 * Process an asset file.
 * 
 * @param sourcePath Path to source file
 * @param type Asset type
 * @return true if successful
 */
public boolean processAsset(String sourcePath, AssetType type) {
    // Implementation
}
```

## Project Structure

```
PolyGameX/
├── src/          # C++ source files
├── include/      # C++ headers
├── scripts/      # Python modules
├── platform/     # Java modules
├── examples/     # Demo applications
└── docs/         # Documentation
```

## Building

See [README.md](README.md) for build instructions.

## Testing

- Write unit tests for new features
- Ensure all tests pass before submitting PR
- Test on multiple platforms if possible

## Documentation

- Update README.md for user-facing changes
- Update ARCHITECTURE.md for design changes
- Add code comments for complex logic
- Update API documentation

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Questions?

Feel free to open an issue for any questions or clarifications.

Thank you for contributing to PolyGameX! 🎮
