#pragma once

#include <string>
#include <fstream>
#include <vector>

namespace PolyGameX {

/**
 * File system utilities for asset loading and management.
 */
class FileSystem {
public:
    /**
     * Read entire file into string.
     */
    static std::string ReadFile(const std::string& filepath);
    
    /**
     * Read binary file into byte array.
     */
    static std::vector<unsigned char> ReadBinaryFile(const std::string& filepath);
    
    /**
     * Write string to file.
     */
    static bool WriteFile(const std::string& filepath, const std::string& content);
    
    /**
     * Write binary data to file.
     */
    static bool WriteBinaryFile(const std::string& filepath, const std::vector<unsigned char>& data);
    
    /**
     * Check if file exists.
     */
    static bool FileExists(const std::string& filepath);
    
    /**
     * Check if directory exists.
     */
    static bool DirectoryExists(const std::string& path);
    
    /**
     * Create directory (and parent directories if needed).
     */
    static bool CreateDirectory(const std::string& path);
    
    /**
     * Get file extension.
     */
    static std::string GetExtension(const std::string& filepath);
    
    /**
     * Get filename without extension.
     */
    static std::string GetFilenameWithoutExtension(const std::string& filepath);
    
    /**
     * Get directory from path.
     */
    static std::string GetDirectory(const std::string& filepath);
    
    /**
     * Join path components.
     */
    static std::string JoinPath(const std::string& a, const std::string& b);
};

} // namespace PolyGameX
