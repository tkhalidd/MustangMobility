#ifndef ENTRANCENODE_H
#define ENTRANCENODE_H

#include <string>

class EntranceNode {
private:
    double latitude;
    double longitude;
    std::string buildingName;
    std::string entranceName;
    std::string description;

public:
    // Constructor
    EntranceNode(double lat, double lon, std::string bName, std::string eName, std::string desc = "No description");
    EntranceNode() = default;

    // Getters
    double getLatitude() const;
    double getLongitude() const;
    std::string getBuildingName() const;
    std::string getEntranceName() const;
    std::string getDescription() const;

    // Setters
    void setLatitude(double lat);
    void setLongitude(double lon);
    void setBuildingName(const std::string &bName);
    void setEntranceName(const std::string &eName);
    void setDescription(const std::string &desc);

    // Other methods
    bool nearRamp() const;
};

#endif