//
// Created by andja on 9.12.25..
//

#ifndef EDGE_H
#define EDGE_H
#include <qhash.h>

class Node;


class Edge {
private:
    Node* _start;
    Node* _end;
    int _edgeID;
public:
    Edge(Node *start, Node *end) : _start(start), _end(end) {}

    Node* getStart() { return _start; }
    Node* getEnd() { return _end; }
};


#endif //EDGE_H