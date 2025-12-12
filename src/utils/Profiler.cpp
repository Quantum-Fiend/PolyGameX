#include "PolyGameX/Utils/Profiler.h"
#include "PolyGameX/Core/Logger.h"
#include <iomanip>
#include <sstream>

namespace PolyGameX {

Profiler& Profiler::GetInstance() {
    static Profiler instance;
    return instance;
}

void Profiler::BeginProfile(const std::string& name) {
    auto& data = m_ActiveProfiles[name];
    data.start_time = std::chrono::high_resolution_clock::now();
}

void Profiler::EndProfile(const std::string& name) {
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto it = m_ActiveProfiles.find(name);
    if (it == m_ActiveProfiles.end()) {
        Logger::Warning("EndProfile called without BeginProfile: " + name);
        return;
    }
    
    auto& data = it->second;
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - data.start_time
    ).count() / 1000.0; // Convert to milliseconds
    
    data.total_duration_ms += duration;
    data.call_count++;
    data.min_duration_ms = std::min(data.min_duration_ms, duration);
    data.max_duration_ms = std::max(data.max_duration_ms, duration);
    
    // Update results
    auto& result = m_Results[name];
    result.name = name;
    result.duration_ms = data.total_duration_ms;
    result.call_count = data.call_count;
    result.avg_duration_ms = data.total_duration_ms / data.call_count;
    result.min_duration_ms = data.min_duration_ms;
    result.max_duration_ms = data.max_duration_ms;
}

void Profiler::PrintResults() {
    if (m_Results.empty()) {
        Logger::Info("No profiling results available");
        return;
    }
    
    Logger::Info("=== Performance Profiling Results ===");
    Logger::Info("");
    
    // Header
    std::stringstream ss;
    ss << std::left << std::setw(30) << "Name"
       << std::right << std::setw(12) << "Calls"
       << std::setw(12) << "Total (ms)"
       << std::setw(12) << "Avg (ms)"
       << std::setw(12) << "Min (ms)"
       << std::setw(12) << "Max (ms)";
    Logger::Info(ss.str());
    Logger::Info(std::string(90, '-'));
    
    // Results
    for (const auto& [name, result] : m_Results) {
        ss.str("");
        ss << std::left << std::setw(30) << result.name
           << std::right << std::setw(12) << result.call_count
           << std::setw(12) << std::fixed << std::setprecision(3) << result.duration_ms
           << std::setw(12) << std::fixed << std::setprecision(3) << result.avg_duration_ms
           << std::setw(12) << std::fixed << std::setprecision(3) << result.min_duration_ms
           << std::setw(12) << std::fixed << std::setprecision(3) << result.max_duration_ms;
        Logger::Info(ss.str());
    }
    
    Logger::Info("");
}

void Profiler::ClearResults() {
    m_ActiveProfiles.clear();
    m_Results.clear();
}

} // namespace PolyGameX
