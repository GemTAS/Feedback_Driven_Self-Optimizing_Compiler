#pragma once
#include <string>
#include <unordered_map>

class Profiler {
public:
    // Loads the profile data from the given file (e.g. profile.txt)
    bool loadProfile(const std::string& filename);

    // Returns the hit count for a specific label, or 0 if not found
    int getHitCount(const std::string& label) const;

    // Checks if the profile has any data
    bool hasData() const;

    // Returns a map of all labels that meet or exceed the hotThreshold
    std::unordered_map<std::string, int> getHotSpots(int hotThreshold) const;

    // Returns the entire profile data
    const std::unordered_map<std::string, int>& getAllData() const;

private:
    std::unordered_map<std::string, int> profileData;
};
