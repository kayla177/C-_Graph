#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Graph.hpp"
#include "illegal_exception.hpp" // Include illegal_exception header

bool isValidId(const std::string &id) {
    for (char c : id) {
        if (!std::isalnum(c)) {
            return false;
        }
    }
    return true;
}

int main()
{
    Graph graph;
    std::string command;

    while (std::getline(std::cin, command))
    {
        std::istringstream iss(command);
        std::string operation;
        iss >> operation;

        try
        {
            if (operation == "LOAD")
            {
                std::string filename, type;
                iss >> filename >> type;

                std::ifstream infile(filename);
                if (!infile.is_open())
                {
                    std::cout << "failure" << std::endl;
                    continue;
                }

                if (type == "entities")
                {
                    std::string id, name, type;
                    while (infile >> id >> name >> type)
                    {
                        if (!isValidId(id))
                        {
                            throw illegal_exception();
                        }
                        graph.addNode(id, name, type);
                    }
                }
                else if (type == "relationships")
                {
                    std::string source, label, destination;
                    double weight;
                    while (infile >> source >> label >> destination >> weight)
                    {
                        if (!isValidId(source) || !isValidId(destination) || weight <= 0)
                        {
                            throw illegal_exception();
                        }
                        graph.addEdge(source, destination, weight, label);
                    }
                }
                std::cout << "success" << std::endl;
            }
            else if (operation == "RELATIONSHIP")
            {
                std::string sourceId, label, destId;
                double weight;
                iss >> sourceId >> label >> destId >> weight;

                if (!isValidId(sourceId) || !isValidId(destId) || weight <= 0)
                {
                    throw illegal_exception();
                }

                if (graph.addEdge(sourceId, destId, weight, label) == "success")
                {
                    std::cout << "success" << std::endl;
                }
                else
                {
                    std::cout << "failure" << std::endl;
                }
            }
            else if (operation == "ENTITY")
            {
                std::string id, name, type;
                iss >> id >> name >> type;

                if (!isValidId(id))
                {
                    throw illegal_exception();
                }

                graph.addNode(id, name, type);
                std::cout << "success" << std::endl;
            }
            else if (operation == "PRINT")
            {
                std::string id;
                iss >> id;

                if (!isValidId(id))
                {
                    throw illegal_exception();
                }

                graph.printAdjacency(id);
            }
            else if (operation == "DELETE")
            {
                std::string id;
                iss >> id;

                if (!isValidId(id))
                {
                    throw illegal_exception();
                }

                std::cout << graph.removeNode(id) << std::endl;
            }
            else if (operation == "PATH")
            {
                std::string id1, id2;
                iss >> id1 >> id2;

                if (!isValidId(id1) || !isValidId(id2))
                {
                    throw illegal_exception();
                }

                auto result = graph.findPath(id1, id2);
                std::vector<std::string> path = std::get<0>(result);
                double weight = std::get<1>(result);

                if (path.empty() || weight == -1)
                {
                    std::cout << "failure" << std::endl;
                }
                else
                {
                    for (const auto &id : path)
                    {
                        std::cout << id << " ";
                    }
                    std::cout << weight << std::endl;
                }
            }
            else if (operation == "HIGHEST")
            {
                graph.findHighestPath();
            }
            else if (operation == "FINDALL")
            {
                std::string fieldType, fieldValue;
                iss >> fieldType >> fieldValue;
                graph.findAll(fieldType, fieldValue);
            }
            else if (operation == "EXIT")
            {
                break;
            }
            else
            {
                throw illegal_exception();
            }
        }
        catch (const illegal_exception &e)
        {
            std::cout << "illegal argument" << std::endl;
        }
    }

    return 0;
}
