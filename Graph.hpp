#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Node.hpp"
#include "illegal_exception.hpp"
#include <vector>
#include <tuple>
#include <string>

class Graph
{
private:
    std::vector<std::string> nodeIds;                                               // Vector of node IDs
    std::vector<Node> nodes;                                                        // Vector of nodes (aligned with nodeIds)
    std::vector<std::vector<std::tuple<std::string, double, std::string>>> adjList; // Adjacency list

    int getNodeIndex(const std::string &id); // Helper to get node index by ID

public:
    Graph();

    void addNode(const std::string &id, const std::string &name, const std::string &type);
    std::string addEdge(const std::string &sourceId, const std::string &destinationId, double weight, const std::string &label);
    std::string removeNode(const std::string &targetID);
    void printAdjacency(const std::string &targetID);
    std::tuple<std::vector<std::string>, double> findPath(const std::string &sourceId, const std::string &destinationId);
    void findHighestPath();
    void findAll(const std::string &fieldType, const std::string &fieldValue);
    bool isGraphEmpty();
};

#endif // GRAPH_HPP
