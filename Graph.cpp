#include "Graph.hpp"
#include "MaxHeap.hpp"
#include <algorithm>
#include <stdexcept>

Graph::Graph() {}

// Helper to get node index by ID
int Graph::getNodeIndex(const std::string &id)
{
    for (int i = 0; i < nodeIds.size(); ++i)
    {
        if (nodeIds[i] == id)
            return i;
    }
    return -1; // Node not found
}

// Add a new node or update an existing one
void Graph::addNode(const std::string &id, const std::string &name, const std::string &type)
{
    int index = getNodeIndex(id);
    if (index != -1)
    {
        nodes[index].update(name, type); // Update existing node
        return;
    }
    nodeIds.push_back(id);
    nodes.emplace_back(id, name, type);
    adjList.emplace_back(); // Initialize adjacency list for the new node
}

std::string Graph::addEdge(const std::string &sourceId, const std::string &destinationId, double weight, const std::string &label)
{
    int sourceIndex = getNodeIndex(sourceId);
    int destIndex = getNodeIndex(destinationId);

    if (sourceIndex == -1 || destIndex == -1)
    {
        return "failure";
    }

    for (auto &edge : adjList[sourceIndex])
    {
        if (std::get<0>(edge) == destinationId)
        {
            edge = {destinationId, weight, label};
            return "success";
        }
    }

    adjList[sourceIndex].emplace_back(destinationId, weight, label);
    adjList[destIndex].emplace_back(sourceId, weight, label);

    return "success";
}

std::string Graph::removeNode(const std::string &targetID)
{
    if (isGraphEmpty())
    {
        return "failure";
    }
    int targetIndex = getNodeIndex(targetID);

    if (targetIndex == -1)
    {
        return "failure";
    }

    adjList.erase(adjList.begin() + targetIndex);

    for (auto &edges : adjList)
    {
        std::vector<std::tuple<std::string, double, std::string>> updatedEdges;
        for (auto &edge : edges)
        {
            if (std::get<0>(edge) != targetID)
            {
                updatedEdges.push_back(edge);
            }
        }
        edges = std::move(updatedEdges);
    }

    nodeIds.erase(nodeIds.begin() + targetIndex);
    nodes.erase(nodes.begin() + targetIndex);

    return "success";
}

void Graph::printAdjacency(const std::string &targetID)
{
    int targetIndex = getNodeIndex(targetID);

    if (targetIndex == -1)
    {
        std::cout << "failure" << std::endl;
        return;
    }

    if (adjList[targetIndex].empty())
    {
        std::cout << std::endl;
        return;
    }

    for (auto &edge : adjList[targetIndex])
    {
        std::cout << std::get<0>(edge) << " ";
    }
    std::cout << std::endl;
}

std::tuple<std::vector<std::string>, double> Graph::findPath(const std::string &sourceId, const std::string &destinationId)
{
    int sourceIndex = getNodeIndex(sourceId);
    int destIndex = getNodeIndex(destinationId);
    if (sourceIndex == -1 || destIndex == -1)
    {
        return {{}, -1};
    }

    MaxHeap queue;
    std::vector<double> bestWeight(nodeIds.size(), -1);
    std::vector<int> parent(nodeIds.size(), -1);

    queue.insert(0, sourceIndex, -1);

    while (!queue.empty())
    {
        auto top = queue.extractMax(); // Extract max tuple
        double currentWeight = std::get<0>(top);
        int currentNode = std::get<1>(top);
        int parentNode = std::get<2>(top);

        if (currentWeight <= bestWeight[currentNode])
        {
            continue;
        }

        bestWeight[currentNode] = currentWeight;
        parent[currentNode] = parentNode;

        for (auto &edge : adjList[currentNode])
        {
            int neighborIndex = getNodeIndex(std::get<0>(edge));
            double edgeWeight = std::get<1>(edge);

            queue.insert(currentWeight + edgeWeight, neighborIndex, currentNode);
        }
    }

    if (bestWeight[destIndex] == -1)
    {
        return {{}, -1};
    }

    // Reverse the path to get source -> destination
    std::vector<std::string> path;
    for (int at = destIndex; at != -1; at = parent[at])
    {
        path.push_back(nodeIds[at]);
    }
    std::reverse(path.begin(), path.end());

    return {path, bestWeight[destIndex]};
}

void Graph::findHighestPath()
{
    if (isGraphEmpty())
    {
        std::cout << "failure" << std::endl;
        return;
    }

    double maxWeight = -1;
    std::string sourceVertex;
    std::string destVertex;

    for (int i = 0; i < nodeIds.size(); ++i)
    {
        for (int j = i + 1; j < nodeIds.size(); ++j)
        {
            auto result = findPath(nodeIds[i], nodeIds[j]);
            std::vector<std::string> path = std::get<0>(result);
            double weight = std::get<1>(result);

            if (weight > maxWeight)
            {
                maxWeight = weight;
                sourceVertex = nodeIds[i];
                destVertex = nodeIds[j];
            }
        }
    }

    if (maxWeight == -1)
    {
        std::cout << "failure" << std::endl;
    }
    else
    {
        std::cout << sourceVertex << " " << destVertex << " " << maxWeight << std::endl;
    }
}

void Graph::findAll(const std::string &fieldType, const std::string &fieldValue)
{
    std::vector<std::string> results;

    if (fieldType == "name")
    {
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            if (nodes[i].getName() == fieldValue)
            {
                results.push_back(nodeIds[i]);
            }
        }
    }
    else if (fieldType == "type")
    {
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            if (nodes[i].getType() == fieldValue)
            {
                results.push_back(nodeIds[i]);
            }
        }
    }
    else
    {
        std::cout << "failure" << std::endl;
        return;
    }

    if (results.empty())
    {
        std::cout << "failure" << std::endl;
    }
    else
    {
        for (const auto &id : results)
        {
            std::cout << id << " ";
        }
        std::cout << std::endl;
    }
}

bool Graph::isGraphEmpty()
{
    // Check if there are no nodes
    if (nodeIds.empty())
        return true;

    // Check if there are no edges (all adjacency lists are empty)
    for (const auto &neighbors : adjList)
    {
        if (!neighbors.empty())
            return false; // Found an edge
    }

    return true; // No edges
}
