#include "Graph.hpp"
#include "MaxHeap.hpp"
#include <algorithm>
#include <tuple>
#include <iostream>
#include "illegal_exception.hpp"

// default constructor
Graph::Graph() {}

int Graph::getNodeIndex(const std::string &id)
{
    // check if the index of a node exist in the nodeId vector
    for (int i = 0; i < nodeIds.size(); ++i)
    {
        if (nodeIds[i] == id)
        {
            // return the corresponding index
            return i;
        }
    }
    // node not found
    return -1;
}

void Graph::addNode(const std::string &id, const std::string &name, const std::string &type)
{
    if (id.empty() || name.empty() || type.empty())
    {
        throw illegal_exception();
    }

    // get the node index from nodeId vector
    int index = getNodeIndex(id);
    // if the node already exist, just undate the value
    if (index != -1)
    {
        nodes[index].update(name, type);
        return;
    }

    // if node not found, create a new node and push that node id onto the nodeIds vector
    nodeIds.push_back(id);
    nodes.emplace_back(id, name, type);
    while (adjList.size() < nodeIds.size())
    {
        adjList.emplace_back();
    }
}

std::string Graph::addEdge(const std::string &sourceId, const std::string &destinationId, double weight, const std::string &label)
{
    if (sourceId == destinationId || weight <= 0)
    {
        throw illegal_exception();
    }

    int sourceIndex = getNodeIndex(sourceId);
    int destIndex = getNodeIndex(destinationId);

    // if either entity does not exist in the graph
    if (sourceIndex == -1 || destIndex == -1)
    {
        return "failure";
    }

    // update edge
    for (auto &edge : adjList[sourceIndex])
    {
        // traverse the edges of the source node, check if an edge to the destination node exist
        if (std::get<0>(edge) == destIndex)
        {
            // update the existing edge info
            edge = {destIndex, weight, label};
            for (auto &reverseEdge : adjList[destIndex])
            {
                // update the edge info from the destination to source
                if (std::get<0>(reverseEdge) == sourceIndex)
                {
                    reverseEdge = {sourceIndex, weight, label};
                    break;
                }
            }
            return "success";
        }
    }

    // create a new relation from source to destination
    adjList[sourceIndex].emplace_back(destIndex, weight, label);
    // create a new relation from destination to source
    adjList[destIndex].emplace_back(sourceIndex, weight, label);

    return "success";
}

std::string Graph::removeNode(const std::string &targetID)
{
    int targetIndex = getNodeIndex(targetID);

    if (targetIndex < 0 || targetIndex >= adjList.size())
    {
        return "failure";
    }

    if (nodeIds.size() != adjList.size())
    {
        return "failure";
    }

    // remove target node's adjacency list, nodeId, and node object from the corresponding list
    adjList.erase(adjList.begin() + targetIndex);
    nodeIds.erase(nodeIds.begin() + targetIndex);
    nodes.erase(nodes.begin() + targetIndex);

    // traverse to delete all edge related to target node
    for (auto &edges : adjList)
    {
        std::vector<std::tuple<int, double, std::string>> updatedEdges;
        // traverse each edge in the adjacent list
        for (auto &edge : edges)
        {
            // add each edge that is NOT connected to the target node to the new edge list (updated edges)
            int neighborIndex = std::get<0>(edge);
            if (neighborIndex == targetIndex)
            {
                continue;
            }
            else if (neighborIndex > targetIndex)
            {
                // in case the node come after the removed node, adjust the index
                updatedEdges.push_back({neighborIndex - 1, std::get<1>(edge), std::get<2>(edge)});
            }
            else
            {
                updatedEdges.push_back(edge);
            }
        }
        // update the edgelist (without edge connect to removed node)
        edges = std::move(updatedEdges);
    }

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

    // if the vertice does not have any edge, print empty line
    if (adjList[targetIndex].empty())
    {
        std::cout << std::endl;
        return;
    }

    // print the id of adjacent node
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
    // if either node does not exist, return an empty path and weight of -1.
    if (sourceIndex == -1 || destIndex == -1)
    {
        return std::make_tuple(std::vector<std::string>(), -1.0);
    }

    // initialize a max heap priority queue
    MaxHeap queue;
    std::vector<double> largestWeight(nodeIds.size(), -1);
    std::vector<int> parent(nodeIds.size(), -1);
    std::vector<bool> visited(nodeIds.size(), false);

    // insert the starting node into the heap, initialize the weight to 0
    queue.insert(0, sourceIndex, -1);
    largestWeight[sourceIndex] = 0;

    while (!queue.empty())
    {
        // get the node that has largest weight from the priority queue
        auto top = queue.extractMax();
        // store weight and index of the current node
        double currentWeight = std::get<0>(top);
        int currentNode = std::get<1>(top);

        // if current node is visited, skip it
        if (visited[currentNode])
        {
            continue;
        }

        // update the visit status of current node
        visited[currentNode] = true;

        if (currentNode == destIndex)
        {
            break;
        }

        for (auto &edge : adjList[currentNode])
        {
            // store the index of the neighboring node and the edge weight to that node
            int neighborIndex = std::get<0>(edge);
            double edgeWeight = std::get<1>(edge);

            // access the neighbor only if it has not been visited
            if (!visited[neighborIndex])
            {
                // calculate the new weight to the neighbor
                double newWeight = currentWeight + edgeWeight;

                // update the largestWeight if the new weight is larger
                if (newWeight > largestWeight[neighborIndex])
                {
                    largestWeight[neighborIndex] = newWeight;
                    // set current node as parent of the neighbor
                    parent[neighborIndex] = currentNode;
                    queue.insert(newWeight, neighborIndex, currentNode);
                }
            }
        }
    }

    // if cannot reach the destination node, return empty path
    if (largestWeight[destIndex] == -1)
    {
        return std::make_tuple(std::vector<std::string>(), -1.0);
    }

    std::vector<std::string> path;
    for (int at = destIndex; at != -1; at = parent[at])
    {
        path.push_back(nodeIds[at]);
    }
    // reverse the path for correct order
    std::reverse(path.begin(), path.end());

    // return the path and total weight
    return std::make_tuple(path, largestWeight[destIndex]);
}

void Graph::findHighestPath()
{
    // check if the graph is empty
    if (isGraphEmpty())
    {
        std::cout << "failure" << std::endl;
        return;
    }

    double maxWeight = -1;
    std::string sourceVertex;
    std::string destVertex;

    // iterate through all the nodes in the graph
    for (int i = 0; i < nodeIds.size(); ++i)
    {
        for (int j = i + 1; j < nodeIds.size(); ++j)
        {
            // find largest weight path between two node
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

    // if no path was found, return failure
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
    // store the ID of node
    std::vector<std::string> results;

    // if need to search by the node's name
    if (fieldType == "name")
    {
        for (int i = 0; i < nodes.size(); ++i)
        {
            // check if the current node's name matches the user input
            if (nodes[i].getName() == fieldValue)
            {
                // add the matching node ID into results
                results.push_back(nodeIds[i]);
            }
        }
    }
    // if need to search by the node's type
    else if (fieldType == "type")
    {
        for (int i = 0; i < nodes.size(); ++i)
        {
            if (nodes[i].getType() == fieldValue)
            {
                results.push_back(nodeIds[i]);
            }
        }
    }
    else
    {
        // if the field typr is not valid
        std::cout << "failure" << std::endl;
        return;
    }

    // if no machting node was found
    if (results.empty())
    {
        std::cout << "failure" << std::endl;
    }
    else
    {
        // print each ID
        for (const auto &id : results)
        {
            std::cout << id << " ";
        }
        std::cout << std::endl;
    }
}

// check if the graph is empty
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
