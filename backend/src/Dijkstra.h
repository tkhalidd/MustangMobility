             cvb#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Graph.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <utility>
#include <queue>
#include <limits>
#include <iostream>
#include <unordered_set>
#include <tuple>

struct PathResult {
    std::vector<std::string> completePathNodes; // Entire path sequence
    double totalDistance;                       // Total path distance
    double totalDuration;                       // Total path duration
};
// Find the shortest path between two nodes in the graph
// Returns a pair containing the total distance and duration
std::pair<double, double> findShortestPath(
    const Graph& graph,           // The graph to search
    const std::string& start,     // Starting node ID
    const std::string& end        // Target node ID

);

// Compute the shortest path through a sequence of buildings
// Handles finding optimal entrance/exit combinations
std::tuple<std::vector<std::string>, double, double> computeShortestPath(
    const Graph& graph,                          // The graph to search
    const std::vector<std::string>& buildings    // Ordered list of buildings to visit
);

#endif