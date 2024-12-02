#include "Node.hpp"

// constructor, initialize the variable based on given value
Node::Node(const std::string &id, const std::string &name, const std::string &type)
{
    this->ID = id;
    this->name = name;
    this->type = type;
}

//---------------------------------- getters -------------------------
std::string Node::getId()
{
    return ID;
}

std::string Node::getName()
{
    return this->name;
}

std::string Node::getType()
{
    return this->type;
}


//---------------------------------- modifiers -------------------------

// update nodes info
void Node::update(std::string newName, std::string newType)
{
    this->name = newName;
    this->type = newType;
}

