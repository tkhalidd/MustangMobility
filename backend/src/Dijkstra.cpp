#include "Dijkstra.h"

// Implementation of Dijkstra's shortest path algorithm for accessible route finding

/**
 * Finds the shortest path between two nodes in the graph
 * @param graph The graph containing all nodes and edges
 * @param start Starting node ID
 * @param validBuildings Set of buildings that can be traversed
 * @param targetBuilding Destination building
 * @param path Vector to store the resulting path
 * @return Pair of total distance and duration
 */
std::pair<double, double> findShortestPath(const Graph& graph, const std::string& start, 
    const std::unordered_set<std::string>& validBuildings, const std::string& targetBuilding, 
    std::vector<std::string>& path) {
    
    // Get references to graph data structures
    const auto& nodes = graph.getNodes();
    const auto& adjList = graph.getAdjacencyList();

    // Initialize data structures for Dijkstra's algorithm
    std::unordered_map<std::string, double> minDistance;  // Track shortest distances
    std::unordered_map<std::string, double> minDuration;  // Track total durations
    std::unordered_map<std::string, std::string> parent;  // Track path predecessors
    std::priority_queue<std::pair<double, std::string>, 
                       std::vector<std::pair<double, std::string>>, 
                       std::greater<>> pq;  // Priority queue for Dijkstra's


    // Validate start node exists
    if (nodes.find(start) == nodes.end()) {
        std::cerr << "Error: Start node " << start << " not found in the graph." << std::endl;
        return {std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()};
    }

    // Initialize distances to infinity
    for (const auto& node : nodes) {
        minDistance[node.first] = std::numeric_limits<double>::infinity();
        minDuration[node.first] = std::numeric_limits<double>::infinity();
    }
    // Set start node distance to 0
    minDistance[start] = 0;
    minDuration[start] = 0;

    // Add start node to priority queue
    pq.push({0, start});

    std::string endNode;
    bool destinationReached = false;

    // Main Dijkstra's algorithm loop
    while (!pq.empty() && !destinationReached) {
        auto [currDist, currNode] = pq.top();
        pq.pop();

        // Skip if we've found a better path already
        if (currDist > minDistance[currNode]) continue;
        if (adjList.find(currNode) == adjList.end()) continue;

        const auto& currentBuilding = nodes.at(currNode).getBuildingName();
        
        // Check if we've reached target building
        if (currentBuilding == targetBuilding) {
            endNode = currNode;
            destinationReached = true;
            break;
        }

        // Process all edges from current node
        for (const auto& edge : adjList.at(currNode)) {
            const auto& nextNode = edge.getNodeId2().getEntranceName();
            const auto& nextBuilding = edge.getNodeId2().getBuildingName();

            // Validate if this path is allowed (same building or in valid buildings list)
            bool isValidPath = validBuildings.find(nextBuilding) != validBuildings.end() || 
                             nextBuilding == currentBuilding;

            if (!isValidPath) continue;

            // Calculate new distances
            double newDist = minDistance[currNode] + edge.getDistance();
            double newDur = minDuration[currNode] + edge.getDuration();

            // Update if new path is shorter
            if (newDist < minDistance[nextNode]) {
                minDistance[nextNode] = newDist;
                minDuration[nextNode] = newDur;
                parent[nextNode] = currNode;
                pq.push({newDist, nextNode});
            }
        }
    }

    // Reconstruct the path
    path.clear();
    std::string currentNode = endNode;
    while (currentNode != start && !currentNode.empty()) {
        path.push_back(currentNode);
        currentNode = parent[currentNode];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());


    return {minDistance[endNode], minDuration[endNode]};
}

/**
 * Computes the complete path through multiple buildings
 * @param graph The graph containing all nodes and edges
 * @param buildings Vector of building codes in order of visitation
 * @return Tuple containing path nodes, total distance, and total duration
 */
std::tuple<std::vector<std::string>, double, double> computeShortestPath(
    const Graph& graph,
    const std::vector<std::string>& buildings) {
        
    // Get references to graph data structures
    const auto& nodes = graph.getNodes();

    // Initialize tracking variables
    double totalDistance = 0;
    double totalDuration = 0;
    std::string currentStartNode;  // Keeps track of where we are in the path
    std::vector<std::string> completePathNodes;  // Stores the entire path sequence

    // Iterate through each pair of consecutive buildings
    for (size_t i = 0; i < buildings.size() - 1; ++i) {
        const std::string& startBuilding = buildings[i];
        const std::string& endBuilding = buildings[i + 1];

        // Set up valid buildings for this segment (only allow paths through start and end buildings)
        std::unordered_set<std::string> validBuildings = {startBuilding, endBuilding};

        // Track the best path for this segment
        double minPathDist = std::numeric_limits<double>::infinity();
        double minPathTime = std::numeric_limits<double>::infinity();
        std::vector<std::string> currentPath;

        // If we have a current position (not the first segment)
        if (!currentStartNode.empty()) {
            // Find path from current position to next building
            auto result = findShortestPath(graph, currentStartNode, validBuildings, endBuilding, currentPath);
            if (result.first < minPathDist) {
                minPathDist = result.first;
                minPathTime = result.second;
                // Add new nodes to complete path, skipping the first node if not the first segment
                // (avoids duplicating nodes where segments connect)
                completePathNodes.insert(completePathNodes.end(),
                    i == 0 ? currentPath.begin() : currentPath.begin() + 1,
                    currentPath.end());
            }
        } else {
            // For first segment, try all possible entrances of the start building
            for (const auto& [startKey, startNode] : nodes) {
                if (startNode.getBuildingName() == startBuilding) {
                    auto result = findShortestPath(graph, startKey, validBuildings, endBuilding, currentPath);
                    if (result.first < minPathDist) {
                        minPathDist = result.first;
                        minPathTime = result.second;
                        completePathNodes = currentPath;  // For first segment, use entire path
                        currentStartNode = startKey;
                    }
                }
            }
        }

        // If a valid path was found for this segment
        if (minPathDist < std::numeric_limits<double>::infinity()) {
            // Add segment metrics to totals
            totalDistance += minPathDist;
            totalDuration += minPathTime;
            // Update current position for next iteration
            currentStartNode = currentPath.back();
        }
    }


    return {completePathNodes, totalDistance, totalDuration};
}

