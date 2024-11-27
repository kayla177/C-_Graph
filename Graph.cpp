// #include "Graph.hpp"
// #include "MaxHeap.hpp"
// #include <algorithm>
// #include <stdexcept>
// #include <iostream>
// #include "illegal_exception.hpp"

// Graph::Graph() {}

// int Graph::getNodeIndex(const std::string &id)
// {
//     for (int i = 0; i < nodeIds.size(); ++i)
//     {
//         if (nodeIds[i] == id)
//         {
//             return i;
//         }
//     }
//     // node not found
//     return -1;
// }

// // Add a new node or update an existing one
// void Graph::addNode(const std::string &id, const std::string &name, const std::string &type)
// {
//     if (id.empty() || name.empty() || type.empty())
//     {
//         throw illegal_exception();
//     }

//     int index = getNodeIndex(id);
//     if (index != -1)
//     {
//         nodes[index].update(name, type); // Update existing node
//         return;
//     }
//     nodeIds.push_back(id);
//     nodes.emplace_back(id, name, type);
//     adjList.emplace_back(); // Initialize adjacency list for the new node
// }

// std::string Graph::addEdge(const std::string &sourceId, const std::string &destinationId, double weight, const std::string &label)
// {
//     if (sourceId == destinationId || weight <= 0)
//     {
//         throw illegal_exception();
//     }

//     int sourceIndex = getNodeIndex(sourceId);
//     int destIndex = getNodeIndex(destinationId);

//     if (sourceIndex == -1 || destIndex == -1)
//     {
//         return "failure";
//     }

//     // Update or add the edge if it already exists
//     for (auto &edge : adjList[sourceIndex])
//     {
//         if (std::get<0>(edge) == destinationId)
//         {
//             edge = {destinationId, weight, label};
//             return "success";
//         }
//     }

//     // Add bidirectional edges
//     adjList[sourceIndex].emplace_back(destinationId, weight, label);
//     adjList[destIndex].emplace_back(sourceId, weight, label);

//     return "success";
// }

// std::string Graph::removeNode(const std::string &targetID)
// {
//     if (isGraphEmpty())
//     {
//         return "failure";
//     }
//     int targetIndex = getNodeIndex(targetID);

//     if (targetIndex == -1)
//     {
//         return "failure";
//     }

//     adjList.erase(adjList.begin() + targetIndex);

//     for (auto &edges : adjList)
//     {
//         std::vector<std::tuple<std::string, double, std::string>> updatedEdges;
//         for (auto &edge : edges)
//         {
//             if (std::get<0>(edge) != targetID)
//             {
//                 updatedEdges.push_back(edge);
//             }
//         }
//         edges = std::move(updatedEdges);
//     }

//     nodeIds.erase(nodeIds.begin() + targetIndex);
//     nodes.erase(nodes.begin() + targetIndex);

//     return "success";
// }

// void Graph::printAdjacency(const std::string &targetID)
// {
//     int targetIndex = getNodeIndex(targetID);

//     if (targetIndex == -1)
//     {
//         std::cout << "failure" << std::endl;
//         return;
//     }

//     if (adjList[targetIndex].empty())
//     {
//         std::cout << std::endl;
//         return;
//     }

//     for (auto &edge : adjList[targetIndex])
//     {
//         std::cout << std::get<0>(edge) << " ";
//     }
//     std::cout << std::endl;
// }

// std::tuple<std::vector<std::string>, double> Graph::findPath(const std::string &sourceId, const std::string &destinationId)
// {
//     int sourceIndex = getNodeIndex(sourceId);
//     int destIndex = getNodeIndex(destinationId);
//     if (sourceIndex == -1 || destIndex == -1)
//     {
//         return {{}, -1};
//     }
//     std::cout << "***********source index: " << sourceIndex << " &&  destIndex:  " << destIndex << std::endl;

//     MaxHeap queue;
//     std::vector<double> bestWeight(nodeIds.size(), -1);
//     std::vector<int> parent(nodeIds.size(), -1);
//     std::vector<bool> visited(nodeIds.size(), false);

//     queue.insert(0, sourceIndex, -1);
//     bestWeight[sourceIndex] = 0;

//     while (!queue.empty())
//     {
//         auto top = queue.extractMax();
//         double currentWeight = std::get<0>(top);
//         int currentNode = std::get<1>(top);
//         int parentNode = std::get<2>(top);

//         std::cout << "***********Extracted Node: " << nodeIds[currentNode]
//                   << ", Weight: " << currentWeight
//                   << ", Parent Node: " << (parentNode != -1 ? nodeIds[parentNode] : "None") << std::endl;

//         if (visited[currentNode])
//         {
//             continue;
//         }

//         visited[currentNode] = true;
//         parent[currentNode] = parentNode;

//         if (currentNode == destIndex)
//         {
//             break;
//         }

//         for (auto &edge : adjList[currentNode])
//         {
//             int neighborIndex = getNodeIndex(std::get<0>(edge));
//             double edgeWeight = std::get<1>(edge);

//             std::cout << "***********Visiting Neighbor: " << nodeIds[neighborIndex]
//                       << ", Edge Weight: " << edgeWeight << std::endl;

//             if (!visited[neighborIndex])
//             {
//                 double newWeight = currentWeight + edgeWeight;

//                 if (newWeight > bestWeight[neighborIndex])
//                 {
//                     bestWeight[neighborIndex] = newWeight;
//                     queue.insert(newWeight, neighborIndex, currentNode);
//                 }
//             }
//         }
//     }

//     if (bestWeight[destIndex] != -1)
//     {
//         std::cout << "***********Path found from " << sourceId << " to " << destinationId
//                   << ", Weight: " << bestWeight[destIndex] << std::endl;
//     }
//     else
//     {
//         std::cout << "***********No valid path found from " << sourceId << " to " << destinationId << std::endl;
//     }

//     if (bestWeight[destIndex] == -1)
//     {
//         return {{}, -1};
//     }

//     std::vector<std::string> path;
//     for (int at = destIndex; at != -1; at = parent[at])
//     {
//         path.push_back(nodeIds[at]);
//     }
//     std::reverse(path.begin(), path.end());

//     return {path, bestWeight[destIndex]};
// }

// void Graph::findHighestPath()
// {
//     std::cout << "***********Starting to find the highest path..." << std::endl;

//     if (isGraphEmpty())
//     {
//         std::cout << "failure" << std::endl;
//         return;
//     }

//     double maxWeight = -1;
//     std::string sourceVertex;
//     std::string destVertex;

//     for (int i = 0; i < nodeIds.size(); ++i)
//     {
//         for (int j = i + 1; j < nodeIds.size(); ++j)
//         {
//             std::cout << "***********Finding path between: " << nodeIds[i] << " and " << nodeIds[j] << std::endl;

//             auto result = findPath(nodeIds[i], nodeIds[j]);
//             std::vector<std::string> path = std::get<0>(result);
//             double weight = std::get<1>(result);

//             if (weight > maxWeight)
//             {
//                 std::cout << "***********New highest weight found: " << weight << " for path between "
//                           << nodeIds[i] << " and " << nodeIds[j] << std::endl;
//                 maxWeight = weight;
//                 sourceVertex = nodeIds[i];
//                 destVertex = nodeIds[j];
//             }
//         }
//     }

//     if (maxWeight == -1)
//     {
//         std::cout << "***********No valid highest path found." << std::endl;
//     }
//     else
//     {
//         std::cout << "***********Highest path found from " << sourceVertex << " to " << destVertex
//                   << " with weight " << maxWeight << std::endl;
//     }

//     if (maxWeight == -1)
//     {
//         std::cout << "failure" << std::endl;
//     }
//     else
//     {
//         std::cout << sourceVertex << " " << destVertex << " " << maxWeight << std::endl;
//     }
// }

// void Graph::findAll(const std::string &fieldType, const std::string &fieldValue)
// {
//     std::vector<std::string> results;

//     if (fieldType == "name")
//     {
//         for (size_t i = 0; i < nodes.size(); ++i)
//         {
//             if (nodes[i].getName() == fieldValue)
//             {
//                 results.push_back(nodeIds[i]);
//             }
//         }
//     }
//     else if (fieldType == "type")
//     {
//         for (size_t i = 0; i < nodes.size(); ++i)
//         {
//             if (nodes[i].getType() == fieldValue)
//             {
//                 results.push_back(nodeIds[i]);
//             }
//         }
//     }
//     else
//     {
//         std::cout << "failure" << std::endl;
//         return;
//     }

//     if (results.empty())
//     {
//         std::cout << "failure" << std::endl;
//     }
//     else
//     {
//         for (const auto &id : results)
//         {
//             std::cout << id << " ";
//         }
//         std::cout << std::endl;
//     }
// }

// bool Graph::isGraphEmpty()
// {
//     if (nodeIds.empty())
//         return true;

//     for (const auto &neighbors : adjList)
//     {
//         if (!neighbors.empty())
//             return false;
//     }

//     return true;
// }


#include "Graph.hpp"
#include "MaxHeap.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include "illegal_exception.hpp"

Graph::Graph() {}

int Graph::getNodeIndex(const std::string &id)
{
    for (int i = 0; i < nodeIds.size(); ++i)
    {
        if (nodeIds[i] == id)
        {
            return i;
        }
    }
    // node not found
    return -1;
}

// Add a new node or update an existing one
void Graph::addNode(const std::string &id, const std::string &name, const std::string &type)
{
    if (id.empty() || name.empty() || type.empty())
    {
        throw illegal_exception();
    }

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
    if (sourceId == destinationId || weight <= 0)
    {
        throw illegal_exception();
    }

    int sourceIndex = getNodeIndex(sourceId);
    int destIndex = getNodeIndex(destinationId);

    if (sourceIndex == -1 || destIndex == -1)
    {
        return "failure";
    }

    // Update or add the edge if it already exists
    for (auto &edge : adjList[sourceIndex])
    {
        if (std::get<0>(edge) == destIndex)
        {
            edge = {destIndex, weight, label};
            return "success";
        }
    }

    // Add bidirectional edges
    adjList[sourceIndex].emplace_back(destIndex, weight, label);
    adjList[destIndex].emplace_back(sourceIndex, weight, label);

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
        std::vector<std::tuple<int, double, std::string>> updatedEdges;
        for (auto &edge : edges)
        {
            if (std::get<0>(edge) != targetIndex)
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
        std::cout << nodeIds[std::get<0>(edge)] << " ";
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
    // std::cout << "source index: " << sourceIndex << " &&  destIndex:  " << destIndex << std::endl;

    MaxHeap queue;
    std::vector<double> bestWeight(nodeIds.size(), -1);
    std::vector<int> parent(nodeIds.size(), -1);
    std::vector<bool> visited(nodeIds.size(), false);

    queue.insert(0, sourceIndex, -1);
    bestWeight[sourceIndex] = 0;

    while (!queue.empty())
    {
        auto top = queue.extractMax();
        double currentWeight = std::get<0>(top);
        int currentNode = std::get<1>(top);
        int parentNode = std::get<2>(top);

        // std::cout << "currentWeight: " << currentWeight << " &&  currentNode:  " << currentNode << " && parentNode: " << parentNode << std::endl;

        if (visited[currentNode])
        {
            continue;
        }

        visited[currentNode] = true;
        parent[currentNode] = parentNode;

        if (currentNode == destIndex)
        {
            break;
        }

        for (auto &edge : adjList[currentNode])
        {
            int neighborIndex = std::get<0>(edge);
            double edgeWeight = std::get<1>(edge);

            if (!visited[neighborIndex])
            {
                double newWeight = currentWeight + edgeWeight;

                if (newWeight > bestWeight[neighborIndex])
                {
                    bestWeight[neighborIndex] = newWeight;
                    queue.insert(newWeight, neighborIndex, currentNode);
                }
            }
        }
    }

    if (bestWeight[destIndex] == -1)
    {
        return {{}, -1};
    }

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
    if (nodeIds.empty())
        return true;

    for (const auto &neighbors : adjList)
    {
        if (!neighbors.empty())
            return false;
    }

    return true;
}
