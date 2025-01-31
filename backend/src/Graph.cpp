#include "Graph.h"
#include "PathEdge.h"

// Graph class implementation for accessible path routing

// Static member initialization
Graph* Graph::instance = nullptr;

/**
 * Adds a new node to the graph
 * @param lat Latitude coordinate
 * @param lon Longitude coordinate
 * @param bName Building name
 * @param eName Entrance name
 * @param desc Description of entrance
 */
void Graph::addNode(double lat, double lon, std::string bName, std::string eName, std::string desc) {
    std::string key = eName;

    // Only add node if it doesn't already exist
    if (nodes.find(key) == nodes.end()) {
        EntranceNode node(lat, lon, bName, eName, desc);
        nodes.emplace(key, node);
    } else {
        std::cerr << "Node with key " << key << " already exists." << std::endl;
    }
}

/**
 * Adds a bidirectional edge between two nodes
 * @param nodeId1 First node
 * @param nodeId2 Second node
 * @param distance Physical distance between nodes
 * @param duration Time to travel between nodes
 * @param buildingPair String identifying the building pair
 */
void Graph::addEdge(const EntranceNode& nodeId1, const EntranceNode& nodeId2, 
                   double distance, double duration, const std::string& buildingPair) {
    std::string node1Key = nodeId1.getEntranceName();
    std::string node2Key = nodeId2.getEntranceName();

    // Prevent self-loops
    if (node1Key == node2Key) {
        std::cerr << "Skipping self-loop for node: " << node1Key << std::endl;
        return;
    }

    // Initialize adjacency lists if they don't exist
    if (adjList.find(node1Key) == adjList.end()) {
        adjList[node1Key] = {};
    }
    if (adjList.find(node2Key) == adjList.end()) {
        adjList[node2Key] = {};
    }

    // Check for existing edge from node1 to node2
    auto& edges1 = adjList[node1Key];
    bool edgeExists1 = std::any_of(edges1.begin(), edges1.end(), 
        [&](const PathEdge& edge) {
            return edge.getNodeId2().getEntranceName() == node2Key;
        });

    // Add forward edge if it doesn't exist using Builder
    if (!edgeExists1) {
        PathEdge edge = PathEdgeBuilder()
                        .setNodeId1(nodeId1)
                        .setNodeId2(nodeId2)
                        .setDistance(distance)
                        .setDuration(duration)
                        .setBuildingPair(buildingPair)
                        .build();
        adjList[node1Key].push_back(edge);
    }

    // Check for existing edge from node2 to node1
    auto& edges2 = adjList[node2Key];
    bool edgeExists2 = std::any_of(edges2.begin(), edges2.end(), 
        [&](const PathEdge& edge) {
            return edge.getNodeId2().getEntranceName() == node1Key;
        });

    // Add reverse edge if it doesn't exist using Builder
    if (!edgeExists2) {
        PathEdge reverseEdge = PathEdgeBuilder()
                               .setNodeId1(nodeId2)
                               .setNodeId2(nodeId1)
                               .setDistance(distance)
                               .setDuration(duration)
                               .setBuildingPair(buildingPair)
                               .build();
        adjList[node2Key].push_back(reverseEdge);
    }
}

/**
 * Parses edge data from CSV file
 * @param filename Path to CSV file containing edge data
 */
void Graph::parseEdgeCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    // Skip header line
    std::getline(file, line);

    // Process each line in the CSV
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string startBuilding, startEntrance, endBuilding, endEntrance, 
                    distanceStr, durationStr, buildingPair;
        std::string skip;
        double distance, duration;

        // Parse CSV fields
        std::getline(ss, startBuilding, ',');
        std::getline(ss, skip, ',');
        std::getline(ss, skip, ',');
        std::getline(ss, startEntrance, ',');
        std::getline(ss, endBuilding, ',');
        std::getline(ss, skip, ',');
        std::getline(ss, skip, ',');
        std::getline(ss, endEntrance, ',');
        std::getline(ss, distanceStr, ',');
        std::getline(ss, durationStr, ',');
        
        // Handle quoted building pair
        if (ss.peek() == '"') {
            ss.ignore();
            std::getline(ss, buildingPair, '"');
            ss.ignore();
        }

        // Convert distance and duration to numbers
        try {
            distance = std::stod(distanceStr);
            duration = std::stod(durationStr);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid number format in line: " << line << std::endl;
            continue;
        }

        // Create edge between nodes
        std::string startKey = startEntrance;
        std::string endKey = endEntrance;

        auto itStart = nodes.find(startKey);
        auto itEnd = nodes.find(endKey);

        if (itStart == nodes.end() || itEnd == nodes.end()) {
            std::cerr << "Error: One or both nodes do not exist for edge: " << line << std::endl;
            continue;
        }

        addEdge(itStart->second, itEnd->second, distance, duration, buildingPair);
    }
}

/**
 * Parses node data from CSV file
 * @param filename Path to CSV file containing node data
 */
void Graph::parseNodeCSV(const std::string& filename) {
    std::cerr << "Attempting to parse CSV files..." << std::endl;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    // Skip header line if it exists
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string lat, lon, bname, ename, desc;

        // Parse each line
        std::getline(ss, lat, ',');
        std::getline(ss, lon, ',');
        std::getline(ss, bname, ',');
        std::getline(ss, ename, ',');
        std::getline(ss, desc, ',');


        try {
            // Convert latitude and longitude from string to double
            double latitude = std::stod(lat);
            double longitude = std::stod(lon);

            addNode(latitude, longitude, bname, ename, desc);


        } catch (const std::exception &e) {
            std::cerr << "Error parsing line: " << line << " - " << e.what() << std::endl;
        }
    }
}