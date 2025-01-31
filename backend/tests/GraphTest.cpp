#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/Graph.h"

class MockEntranceNode {
public:
    MOCK_METHOD(std::string, getEntranceName, (), (const));
    MOCK_METHOD(std::string, getBuildingName, (), (const));
    MOCK_METHOD(double, getLatitude, (), (const));
    MOCK_METHOD(double, getLongitude, (), (const));
};

class GraphTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Get a fresh instance for each test
        graph = Graph::getInstance();
    }

    Graph* graph;
};

// Test node addition
TEST_F(GraphTest, AddNodeTest) {
    // Test adding a valid node
    graph->addNode(40.7128, -74.0060, "Building1", "Entrance1_A", "Main entrance");
    auto nodes = graph->getNodes();
    EXPECT_EQ(nodes.size(), 1);
    EXPECT_TRUE(nodes.find("Entrance1_A") != nodes.end());

    // Test adding duplicate node
    graph->addNode(40.7128, -74.0060, "Building1", "Entrance1_A", "Main entrance");
    nodes = graph->getNodes();
    EXPECT_EQ(nodes.size(), 1); // Size should not increase
}

// Test edge addition
TEST_F(GraphTest, AddEdgeTest) {
    // Add two nodes first
    graph->addNode(40.7128, -74.0060, "Building1", "Entrance1_B", "Main entrance");
    graph->addNode(40.7129, -74.0061, "Building2", "Entrance2_B", "Side entrance");

    // Get the nodes
    auto& nodes = graph->getNodes();
    auto& node1 = nodes.at("Entrance1_B");
    auto& node2 = nodes.at("Entrance2_B");

    // Add edge between them
    graph->addEdge(node1, node2, 100.0, 5.0, "Building1-Building2");

    // Verify edge addition
    auto adjList = graph->getAdjacencyList();
    EXPECT_EQ(adjList["Entrance1_B"].size(), 1);
    EXPECT_EQ(adjList["Entrance2_B"].size(), 1); // Bidirectional edge
}

// Test edge case: self-loop prevention
TEST_F(GraphTest, PreventSelfLoopTest) {
    graph->addNode(40.7128, -74.0060, "Building1", "Entrance1_C", "Main entrance");
    auto& nodes = graph->getNodes();
    auto& node1 = nodes.at("Entrance1_C");

    // Attempt to add self-loop
    graph->addEdge(node1, node1, 100.0, 5.0, "Building1-Building1");

    // Verify no edge was added
    auto adjList = graph->getAdjacencyList();
    EXPECT_EQ(adjList["Entrance1_C"].size(), 0);
} 