#pragma once

#include <string>
#include <chrono>
#include <unordered_map>

namespace PolyGameX {

/**
 * Performance profiling utility for measuring execution time.
 * 
 * Usage:
 *   PROFILE_SCOPE("MyFunction");
 *   // code to profile
 */
class POLYGAMEX_API Profiler {
public:
    struct ProfileResult {
        std::string name;
        double duration_ms;
        size_t call_count;
        double avg_duration_ms;
        double min_duration_ms;
        double max_duration_ms;
    };
    
    static Profiler& GetInstance();
    
    void BeginProfile(const std::string& name);
    void EndProfile(const std::string& name);
    
    void PrintResults();
    void ClearResults();
    
    const std::unordered_map<std::string, ProfileResult>& GetResults() const { return m_Results; }
    
private:
    Profiler() = default;
    
    struct ProfileData {
        std::chrono::high_resolution_clock::time_point start_time;
        double total_duration_ms = 0.0;
        size_t call_count = 0;
        double min_duration_ms = std::numeric_limits<double>::max();
        double max_duration_ms = 0.0;
    };
    
    std::unordered_map<std::string, ProfileData> m_ActiveProfiles;
    std::unordered_map<std::string, ProfileResult> m_Results;
};

/**
 * RAII-based profile scope helper.
 */
class ProfileScope {
public:
    ProfileScope(const std::string& name) : m_Name(name) {
        Profiler::GetInstance().BeginProfile(m_Name);
    }
    
    ~ProfileScope() {
        Profiler::GetInstance().EndProfile(m_Name);
    }
    
private:
    std::string m_Name;
};

// Convenience macro
#define PROFILE_SCOPE(name) ProfileScope __profile_scope__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)

} // namespace PolyGameX
