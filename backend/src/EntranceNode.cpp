/**
 * EntranceNode.cpp
 * Represents an accessible entrance point in the campus navigation system
 * Stores location data and accessibility information for building entrances
 */

#include "EntranceNode.h"

/**
 * Constructor for EntranceNode
 * @param lat Latitude coordinate of the entrance
 * @param lon Longitude coordinate of the entrance
 * @param bName Name of the building this entrance belongs to
 * @param eName Unique identifier for this entrance
 * @param desc Description including accessibility features
 */
EntranceNode::EntranceNode(double lat, double lon, std::string bName, 
                         std::string eName, std::string desc)
    : latitude(lat)
    , longitude(lon)
    , buildingName(std::move(bName))
    , entranceName(std::move(eName))
    , description(std::move(desc)) {}

// Geographic Coordinate Getters
double EntranceNode::getLatitude() const {
    return latitude;
}

double EntranceNode::getLongitude() const {
    return longitude;
}

// Identification Getters
std::string EntranceNode::getBuildingName() const {
    return buildingName;
}

std::string EntranceNode::getEntranceName() const {
    return entranceName;
}

std::string EntranceNode::getDescription() const {
    return description;
}

// Property Setters
void EntranceNode::setLatitude(double lat) {
    latitude = lat;
}

void EntranceNode::setLongitude(double lon) {
    longitude = lon;
}

void EntranceNode::setBuildingName(const std::string &bName) {
    buildingName = bName;
}

void EntranceNode::setEntranceName(const std::string &eName) {
    entranceName = eName;
}

void EntranceNode::setDescription(const std::string &desc) {
    description = desc;
}

/**
 * Checks if this entrance has ramp access
 * @return true if description contains "ramp" (case insensitive)
 */
bool EntranceNode::nearRamp() const {
    return description.find("ramp") != std::string::npos ||
           description.find("Ramp") != std::string::npos;
}