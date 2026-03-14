#include "profiler.h"
#include "../utils/logger.h"
#include <fstream>

bool Profiler::loadProfile(const std::string& filename) {
    // Clear out any old profile data before we start a fresh run.
    profileData.clear();

    std::ifstream in(filename);
    if (!in.is_open()) {
        Logger::warn("Profiler: No profile found (" + filename + ") — proceeding without runtime data.");
        return false;
    }

    // Alright, let's parse the file. It's just a simple list of labels and hit counts.
    std::string label;
    int count;
    while (in >> label >> count) {
        profileData[label] = count;
    }

    Logger::info("Profiler: Loaded execution profile with " + std::to_string(profileData.size()) + " data points.");
    return true;
}

int Profiler::getHitCount(const std::string& label) const {
    auto it = profileData.find(label);
    if (it != profileData.end()) {
        return it->second;
    }
    return 0;
}

bool Profiler::hasData() const {
    return !profileData.empty();
}

std::unordered_map<std::string, int> Profiler::getHotSpots(int hotThreshold) const {
    std::unordered_map<std::string, int> hotSpots;
    for (const auto& pair : profileData) {
        if (pair.second >= hotThreshold) {
            hotSpots.insert(pair);
        }
    }
    return hotSpots;
}

const std::unordered_map<std::string, int>& Profiler::getAllData() const {
    return profileData;
}
