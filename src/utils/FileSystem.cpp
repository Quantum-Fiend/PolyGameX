#include "PolyGameX/Utils/FileSystem.h"
#include "PolyGameX/Core/Logger.h"
#include <sstream>
#include <filesystem>

namespace PolyGameX {

std::string FileSystem::ReadFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        Logger::Error("Failed to open file: " + filepath);
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<unsigned char> FileSystem::ReadBinaryFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        Logger::Error("Failed to open binary file: " + filepath);
        return {};
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        Logger::Error("Failed to read binary file: " + filepath);
        return {};
    }
    
    return buffer;
}

bool FileSystem::WriteFile(const std::string& filepath, const std::string& content) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        Logger::Error("Failed to create file: " + filepath);
        return false;
    }
    
    file << content;
    return true;
}

bool FileSystem::WriteBinaryFile(const std::string& filepath, const std::vector<unsigned char>& data) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        Logger::Error("Failed to create binary file: " + filepath);
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return true;
}

bool FileSystem::FileExists(const std::string& filepath) {
    return std::filesystem::exists(filepath) && std::filesystem::is_regular_file(filepath);
}

bool FileSystem::DirectoryExists(const std::string& path) {
    return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

bool FileSystem::CreateDirectory(const std::string& path) {
    try {
        return std::filesystem::create_directories(path);
    } catch (const std::exception& e) {
        Logger::Error("Failed to create directory: " + std::string(e.what()));
        return false;
    }
}

std::string FileSystem::GetExtension(const std::string& filepath) {
    std::filesystem::path path(filepath);
    return path.extension().string();
}

std::string FileSystem::GetFilenameWithoutExtension(const std::string& filepath) {
    std::filesystem::path path(filepath);
    return path.stem().string();
}

std::string FileSystem::GetDirectory(const std::string& filepath) {
    std::filesystem::path path(filepath);
    return path.parent_path().string();
}

std::string FileSystem::JoinPath(const std::string& a, const std::string& b) {
    std::filesystem::path path = std::filesystem::path(a) / b;
    return path.string();
}

} // namespace PolyGameX
