//
// Created by andja on 9.12.25..
//

#ifndef EDGE_H
#define EDGE_H
#include <qhash.h>

class Node;


class Edge {
private:
    Node* m_start;
    Node* m_end;
    int m_edgeID;
public:
    Edge(Node *start, Node *end) : m_start(start), m_end(end) {}

    Node* getStart() { return m_start; }
    Node* getEnd() { return m_end; }
};


#endif //EDGE_H
