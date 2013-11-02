#include "graph.h"
#include <iostream>
#include <map>
#include <utility>
#include "coroutine.h"

using namespace std;

namespace {

bool dfsFind( Node *node, int target, Policy policy = Policy::BFS );
bool bfsFind( Node *node, int target, Policy policy = Policy::BFS );

}

Node::Node( int id ) : id( id ), state( BLANK ) {}

bool Node::find( int target, Policy policy ) {

    if ( policy == Policy::DFS ) {
        return dfsFind( this, target, policy );
    }
    else if ( policy == Policy::BFS ) {
        return bfsFind( this, target, policy );
    }

}

Graph::Graph( int numNodes, int rowLength )
: numNodes( numNodes ),
  rowLength( rowLength ),
  begin( 0 ),
  end( numNodes - 1 ) {

    for ( int i = 0; i < numNodes; ++i ) {
        nodeOwner.push_back( new Node( i ) );
    }

    initGraph();

}

Graph::~Graph()
{
    for ( int i = 0; i < numNodes; ++i ) {
        delete nodeOwner[ i ];
    }
}

int Graph::getNumVertices() { return numNodes; }

Node::State Graph::getNodeState( int id ) {
    if ( id >= 0 && id < numNodes ) {

        return nodeOwner[ id ]->state;

    }
    else {

        cerr << "Node { id: "
             << id
             << " } doesn't exist"
             << endl;

        return Node::BLANK;

    }
}

void Graph::setNodeState( int id, Node::State state ) {
    if ( id >= 0 && id < numNodes ) {

        nodeOwner[ id ]->state = state;

    }
    else {

        cerr << "Node { id: "
             << id
             << " } doesn't exist"
             << endl;

    }
}

void Graph::setBegin( int id ) { begin = id; }
void Graph::setEnd  ( int id ) { end   = id; }

void Graph::resetStates() {
    for ( int i = 0; i < numNodes; ++i ) {
        nodeOwner[ i ]->state = Node::BLANK;
    }
}

void Graph::calcPath() {

    Coroutine::_Yield( nullptr );

    for ( auto n : nodeOwner[ begin ]->neighbours ) {

        if ( n->find( end ) ) {
            break;
        }

    }

}

void Graph::crt() {
    while ( true ) {

        //cerr << "Start" << endl;

        calcPath();

        status = INACTIVE;

        Coroutine::_Yield( nullptr );

    }
}

void Graph::initGraph() {

#define ADD_NEIGHBOUR(a, arg) \
    do { \
        if ( \
             ( arg ) >= 0 && ( arg ) < numNodes && \
             ( ( a ) % rowLength == 0 ? ( arg ) != a - 1 : 1 ) && \
             ( ( a ) % rowLength == ( rowLength - 1 ) ? ( arg ) != a + 1 : 1 ) \
           ) { \
            n->neighbours.push_back( nodeOwner[ arg ] ); \
        } \
    } while ( 0 )

    for ( int i = 0; i < numNodes; ++i ) {

        Node *n = nodeOwner[ i ];

        int left  = i - 1;
        int right = i + 1;
        int up    = i - rowLength;
        int down  = i + rowLength;

        ADD_NEIGHBOUR( i, left  );
        ADD_NEIGHBOUR( i, right );
        ADD_NEIGHBOUR( i, up    );
        ADD_NEIGHBOUR( i, down  );

    }

#undef ADD_NEIGHBOUR

}

GraphThread::GraphThread( Graph *graph )
: graph( graph ) {

    refreshCache();

}

void GraphThread::threadedFunction() {

    Coroutine::_InitCurThread( nullptr );

    while ( isThreadRunning() ) {

        graph->resetStates();

        int begin = rand() % graph->getNumVertices();
        int end   = rand() % graph->getNumVertices();

        graph->setBegin( begin );
        graph->setEnd( end );

        graph->setNodeState( begin, Node::BEGIN );
        graph->setNodeState( end,   Node::END   );

        //
        // coroutine call
        //

        Coroutine crt( graph );

        graph->status = Graph::ACTIVE;

        while ( graph->status == Graph::ACTIVE ) {

            if ( !isThreadRunning() ) {
                return;
            }

            crt.start();
            refreshCache();

            sleep( 300 );

        }

        sleep( 1000 );
    }

}

NodeStateVector GraphThread::getStates() {

    lock();

    NodeStateVector cache = stateCache;

    unlock();

    return cache;

}

void GraphThread::refreshCache() {

    lock();

    stateCache.clear();

    for ( int i = 0; i < graph->getNumVertices(); ++i ) {
        stateCache.push_back( graph->getNodeState( i ) );
    }

    unlock();

}

namespace {

bool dfsFind( Node *node, int target, Policy policy ) {

    int         &id         = node->id;
    Node::State &state      = node->state;
    NodeVector  &neighbours = node->neighbours;

    if ( id == target ) {
        state = Node::END;
        return true;
    }

    if ( state != Node::BLANK ) {
        return false;
    }

    state = Node::PEEKING;
    Coroutine::_Yield( nullptr );   // yield to let render take place

    state = Node::VISITING;

    bool isFound = false;

    for ( auto n : neighbours ) {

        if ( ( isFound = n->find( target, policy ) ) ) {
            break;
        }

    }

    state = Node::VISITED;

    if ( isFound ) {
        state = Node::PATH;
    }

    return isFound;

}

bool bfsFind( Node *node, int target, Policy policy ) {
}

}
