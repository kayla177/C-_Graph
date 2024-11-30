#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <tuple>
#include "Node.hpp"

class Graph {
private:
    std::vector<std::string> nodeIds;
    std::vector<Node> nodes;
    std::vector<std::vector<std::tuple<int, double, std::string>>> adjList;

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
    int getNodeIndex(const std::string &id);
};

#endif
