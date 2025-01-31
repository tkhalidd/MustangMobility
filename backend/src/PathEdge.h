#ifndef PATHEDGE_H
#define PATHEDGE_H

#include "EntranceNode.h"
#include <string>

class PathEdge {
private:
    // The two nodes this edge connects
    EntranceNode nodeId1;  // Starting node
    EntranceNode nodeId2;  // Ending node
    
    // Edge properties
    double distance;       // Distance in meters
    double duration;       // Time to traverse in seconds
    std::string buildingPair;  // Identifier for the building pair this edge connects

public:
    // Constructor
    PathEdge() = default;
    PathEdge(const EntranceNode& n1, const EntranceNode& n2, 
            double dist, double dur, const std::string& pair);

    // Getters
    EntranceNode getNodeId1() const;
    EntranceNode getNodeId2() const;
    double getDistance() const;
    double getDuration() const;
    std::string getBuildingPair() const;

    // Setters
    void setNodeId1(const EntranceNode& n1);
    void setNodeId2(const EntranceNode& n2);
    void setDistance(double dist);
    void setDuration(double dur);
    void setBuildingPair(const std::string& pair);

    // Utility methods
    bool isInternal() const;  // Check if edge is within same building
    bool isAccessible() const;  // Check if path is accessible
};

class PathEdgeBuilder {
public:
    PathEdgeBuilder() = default;  // Explicitly declare default constructor
    
    // Setter methods for each property
    PathEdgeBuilder& setNodeId1(const EntranceNode& node) {
        nodeId1 = node;
        return *this;
    }

    PathEdgeBuilder& setNodeId2(const EntranceNode& node) {
        nodeId2 = node;
        return *this;
    }

    PathEdgeBuilder& setDistance(double dist) {
        distance = dist;
        return *this;
    }

    PathEdgeBuilder& setDuration(double dur) {
        duration = dur;
        return *this;
    }

    PathEdgeBuilder& setBuildingPair(const std::string& pair) {
        buildingPair = pair;
        return *this;
    }

    // Method to build the PathEdge object
    PathEdge build() const {
        return PathEdge(nodeId1, nodeId2, distance, duration, buildingPair);
    }

private:
    EntranceNode nodeId1;
    EntranceNode nodeId2;
    double distance = 0.0;
    double duration = 0.0;
    std::string buildingPair;
};

#endif // PATHEDGE_H
