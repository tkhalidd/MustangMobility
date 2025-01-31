/**
 * main.cpp
 * Entry point for the Mustang Mobility accessible path finding system
 * Handles graph initialization, data loading, and path computation
 */

#include <iostream>
#include "Graph.h"
#include "Dijkstra.h"

/**
 * Main program entry point
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments (building codes)
 * @return 0 on successful execution
 */
int main(int argc, char* argv[]) {
    // Initialize graph singleton instance
    Graph* graph = Graph::getInstance();

    // Load campus data from CSV files
    // Note: Update paths based on deployment environment
    graph->parseNodeCSV("../data/accessibleentrances.csv");
    graph->parseEdgeCSV("../data/precalculated_distances_with_building_codes.csv");

    // Process command line arguments into building sequence
    std::vector<std::string> buildings;
    for (int i = 1; i < argc; ++i) { // Skip argv[0] (program name)
        buildings.emplace_back(argv[i]);
    }

    // Calculate optimal accessible path through specified buildings
    auto [completePathNodes, totalDistance, totalDuration] = computeShortestPath(*graph, buildings);

    // Output results
    std::cout << "Complete Path Nodes: ";
    for (const std::string& node : completePathNodes) {
        std::cout << node << " ";
    }
    std::cout << "\nTotal Distance: " << totalDistance << std::endl;
    std::cout << "Total Duration: " << totalDuration << std::endl;

    return 0;
}