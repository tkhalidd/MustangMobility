#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/Dijkstra.h"

class MockGraph {
public:
    MOCK_METHOD((const std::unordered_map<std::string, EntranceNode>&), getNodes, (), (const));
    MOCK_METHOD((const std::unordered_map<std::string, std::vector<PathEdge>>&), getAdjacencyList, (), (const));
};

class DijkstraTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Get a fresh instance for each test
        graph = Graph::getInstance();
        setupTestGraph();
    }

    void setupTestGraph() {
        // Add test nodes with unique names
        graph->addNode(40.7128, -74.0060, "BuildingA", "EntranceA1_D", "Main");
        graph->addNode(40.7129, -74.0061, "BuildingA", "EntranceA2_D", "Side");
        graph->addNode(40.7130, -74.0062, "BuildingB", "EntranceB1_D", "Main");
        
        // Add test edges
        auto& nodes = graph->getNodes();
        graph->addEdge(nodes.at("EntranceA1_D"), nodes.at("EntranceA2_D"), 100.0, 2.0, "BuildingA-BuildingA");
        graph->addEdge(nodes.at("EntranceA2_D"), nodes.at("EntranceB1_D"), 200.0, 4.0, "BuildingA-BuildingB");
    }

    Graph* graph;
};

// Test basic path finding between two points
TEST_F(DijkstraTest, BasicPathFinding) {
    std::vector<std::string> buildings = {"BuildingA", "BuildingB"};
    auto [path, distance, duration] = computeShortestPath(*graph, buildings);

    EXPECT_FALSE(path.empty());
    EXPECT_LT(distance, std::numeric_limits<double>::infinity());
    EXPECT_LT(duration, std::numeric_limits<double>::infinity());
}

// Test path finding with invalid buildings
TEST_F(DijkstraTest, InvalidBuildingPath) {
    std::vector<std::string> buildings = {"BuildingA", "NonexistentBuilding"};
    auto [path, distance, duration] = computeShortestPath(*graph, buildings);

    EXPECT_TRUE(path.empty() || path.size() == 1); // Should only contain start node if any
    EXPECT_EQ(distance, 0.0);
    EXPECT_EQ(duration, 0.0);
}

// Test path finding between multiple buildings
TEST_F(DijkstraTest, MultipleBuildings) {
    // Add another building to create a longer path
    graph->addNode(40.7131, -74.0063, "BuildingC", "EntranceC1_D", "Main");
    auto& nodes = graph->getNodes();
    graph->addEdge(nodes.at("EntranceB1_D"), nodes.at("EntranceC1_D"), 150.0, 3.0, "BuildingB-BuildingC");

    std::vector<std::string> buildings = {"BuildingA", "BuildingB", "BuildingC"};
    auto [path, distance, duration] = computeShortestPath(*graph, buildings);

    EXPECT_GT(path.size(), 2); // Should include multiple nodes
    EXPECT_GT(distance, 0.0);
    EXPECT_GT(duration, 0.0);
} 