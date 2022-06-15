#include "node.h"
unsigned Node::sumofNodes = START_SUM;

Node::Node(const Word &param)
{
    sumofNodes++;
    this->word.SetwordDefault();
    if (!word.IswordsEqual(param))
    {
        word = param;
    }
    next = nullptr;
}
Node::Node()
{
    sumofNodes++;
    this->word.SetwordDefault();
    next = nullptr;
}
Node::~Node()
{
    next = nullptr;
    sumofNodes--;
}
unsigned Node::getNodesum()
{
    return Node::sumofNodes;
}
void Node::subfromNodeSum(const int &param)
{
    sumofNodes -= param;
}
/*
std::ostream &operator<<(std::ostream &output, Node *node)
{
    output << "Node's Word: " << node->word.getWord() << "\tNode's Code: " << node->word.getCode() << "\tNode's Word's Count: " << node->word.GetwordCount() << "\tNode's Word's Lenght: " << node->word.GetwordLenght();
    return output;
}
*/