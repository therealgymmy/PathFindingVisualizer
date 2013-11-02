#ifndef GRAPH_H
#define GRAPH_H

#include "ofThread.h"
#include <vector>

struct Node;
class GraphThread;

typedef std::vector< Node* > NodeVector;

enum class Policy { DFS, BFS, A_STAR };

struct Node {

    enum State { BLANK, PEEKING, VISITING, VISITED, PATH, BEGIN, END };

    int        id;
    State      state;
    NodeVector neighbours;

    Node ( int id );

    bool find ( int target, Policy policy = Policy::DFS );

};

typedef std::vector< Node::State > NodeStateVector;

class Graph {
public:

    enum Status { INACTIVE, ACTIVE };

    Graph( int numNodes, int rowLength );
    virtual ~Graph();

    int getNumVertices();

    Node::State getNodeState( int id );
    void setNodeState( int id, Node::State state );

    void setBegin( int id );
    void setEnd( int id );

    void resetStates();
    void calcPath();

    void crt();    // coroutine call

    Status status;

private:
    int        numNodes;
    int        rowLength;
    int        begin;
    int        end;
    NodeVector nodeOwner;
    Node      *root;

    void initGraph();

};

class GraphThread : public ofThread {
public:
    GraphThread( Graph *graph );

    void threadedFunction();

    NodeStateVector getStates();

    void refreshCache();

private:
    Graph          *graph;
    NodeStateVector stateCache;

};

#endif // GRAPH_H
