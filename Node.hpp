#include <iostream>

class Node
{
private:
    std::string ID;
    std::string name;
    std::string type;

public:
    Node(const std::string &id, const std::string &name, const std::string &type);

    //---------------------------------- getters -------------------------
    std::string getId();
    std::string getName();
    std::string getType();

    //---------------------------------- modifiers -------------------------
    void update(std::string newName, std::string newType);
};