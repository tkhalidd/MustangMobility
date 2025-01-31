#ifndef GRAPH_H
#define GRAPH_H

#include "EntranceNode.h"
#include "PathEdge.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <limits>

class Graph {
private:
    // Singleton instance pointer
    static Graph* instance;

    // Map to store nodes by their unique entrance names
    std::unordered_map<std::string, EntranceNode> nodes;

    // Adjacency list to store edges between nodes
    std::unordered_map<std::string, std::vector<PathEdge>> adjList;

    // Map to store building pairs
    std::unordered_map<std::string, std::pair<std::string, std::string>> buildingPairs;

    // Private constructor for singleton pattern
    Graph() = default;

public:
    // Delete copy constructor and assignment operator to maintain singleton
    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;

    // Static method to get the single instance of Graph
    static Graph* getInstance() {
        if (!instance) {
            instance = new Graph();
        }
        return instance;
    }

    // Add a new node to the graph
    void addNode(double lat, double lon, std::string bName, std::string eName, std::string desc);

    // Add a new edge between two nodes
    void addEdge(const EntranceNode& getNodeId1, const EntranceNode& getNodeId2, 
                double distance, double duration, const std::string& buildingPair);

    // Parse CSV files to populate the graph
    void parseEdgeCSV(const std::string& filename);
    void parseNodeCSV(const std::string& filename);

    // Getter methods for graph components
    std::unordered_map<std::string, EntranceNode> const& getNodes() const {
        return nodes;
    }

    std::unordered_map<std::string, std::vector<PathEdge>> const& getAdjacencyList() const {
        return adjList;
    }
};

#endif
