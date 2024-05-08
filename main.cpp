#include "event.h"
#include <iostream>

using namespace simu;

class Info : public Content
{
private:
    int arg;
    std::string str;

public:
    Info(int arg, std::string str);
    ~Info();
    void print();
    static Content *createContent(Object);
};

Info::Info(int arg, std::string str)
{
    this->arg = arg;
    this->str = str;
}

Info::~Info()
{
}

void Info::print()
{
    std::cout << arg << " " << str;
}

Content *Info::createContent(...)
{
    Content *content = new Info(arg, str);
    // wait to init /*arg*/
    return content;
}

class Node : public Object
{
private:
    /* data */
public:
    Node(/* args */);
    ~Node();
    bool processEvent(Content *content);
};

Node::Node(/* args */)
{
}

Node::~Node()
{
}

bool Node::processEvent(Content *content)
{
    reinterpret_cast<Info *>(content)->print();
    return true;
}

int main(int, char **)
{
    Node node0;
    Node node1;
    eventLoop.registerObject(0, &node0);
    eventLoop.registerObject(1, &node1);
    Info *info = Info::createContent(1, "this is info");
    Event event;
    event.creatEvent(Time(1), info, &node0, &node1, 0, 1, "src", "dst");
    eventLoop.pushEvent(event);
    eventLoop.exec(Time(3));
}
