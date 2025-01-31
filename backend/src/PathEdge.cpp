/**
 * PathEdge.cpp
 * Represents a walkable path between two entrance nodes
 * Stores distance, duration, and accessibility information for the path
 */

#include "PathEdge.h"

/**
 * Constructor for PathEdge
 * @param n1 Source entrance node
 * @param n2 Destination entrance node
 * @param dist Physical distance in meters
 * @param dur Walking duration in seconds
 * @param pair Identifier for the building pair connection
 */
PathEdge::PathEdge(const EntranceNode& n1, const EntranceNode& n2, 
                   double dist, double dur, const std::string& pair)
    : nodeId1(n1)
    , nodeId2(n2)
    , distance(dist)
    , duration(dur)
    , buildingPair(pair) {}

// Node Access Getters
EntranceNode PathEdge::getNodeId1() const {
    return nodeId1;
}

EntranceNode PathEdge::getNodeId2() const {
    return nodeId2;
}

// Path Metrics Getters
double PathEdge::getDistance() const {
    return distance;
}

double PathEdge::getDuration() const {
    return duration;
}

std::string PathEdge::getBuildingPair() const {
    return buildingPair;
}

// Property Setters
void PathEdge::setNodeId1(const EntranceNode& n1) {
    nodeId1 = n1;
}

void PathEdge::setNodeId2(const EntranceNode& n2) {
    nodeId2 = n2;
}

void PathEdge::setDistance(double dist) {
    distance = dist;
}

void PathEdge::setDuration(double dur) {
    duration = dur;
}

void PathEdge::setBuildingPair(const std::string& pair) {
    buildingPair = pair;
}

/**
 * Checks if this edge connects entrances within the same building
 * @return true if both nodes belong to the same building
 */
bool PathEdge::isInternal() const {
    return nodeId1.getBuildingName() == nodeId2.getBuildingName();
}

/**
 * Checks if this path is fully accessible
 * @return true if both connected entrances have ramp access
 */
bool PathEdge::isAccessible() const {
    return nodeId1.nearRamp() && nodeId2.nearRamp();
}
