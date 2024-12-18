#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <vector>
#include <tuple>

class Node
{
private:
    std::string ID;
    std::string name;
    std::string type;

public:
    Node(const std::string &id, const std::string &name, const std::string &type);

    std::vector<std::tuple<int, double>> neighbors;

    //---------------------------------- getters -------------------------
    std::string getId();
    std::string getName();
    std::string getType();

    //---------------------------------- modifiers -------------------------
    void update(std::string newName, std::string newType);
};

#endif