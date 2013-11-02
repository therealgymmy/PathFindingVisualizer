#include "testApp.h"

testApp::~testApp() {

    thread->waitForThread();

    delete thread;
    delete graph;

}

//--------------------------------------------------------------
void testApp::setup(){

    ofSetFrameRate( 24 );

    numCubeX = 20;
    numCubeY = 20;

    graph = nullptr;
    graph = new Graph( numCubeX * numCubeY, numCubeX );

    graph->setNodeState( 1,   Node::VISITING );
    graph->setNodeState( 2,   Node::VISITED );
    graph->setNodeState( 5,   Node::VISITING );
    graph->setNodeState( 50,  Node::VISITING );
    graph->setNodeState( 60,  Node::VISITED );
    graph->setNodeState( 200, Node::PATH );

    thread = new GraphThread( graph );

    thread->startThread( true, false );

}

//--------------------------------------------------------------
void testApp::update(){

    windowWidth  = ofGetWidth();
    windowHeight = ofGetHeight();

    cubeWidth  = ( float )windowWidth  / ( float )numCubeX;
    cubeHeight = ( float )windowHeight / ( float )numCubeY;

    cubeBorderSize = cubeWidth / 10.0;

    cubeCornerRadius = cubeWidth / 10.0;

}

//--------------------------------------------------------------
void testApp::draw(){

    NodeStateVector states = thread->getStates();

    for ( int i = 0; i < numCubeX; ++i ) {
        for ( int j = 0; j < numCubeY; ++j ) {

            int id = j * numCubeX + i;

            drawRect( i, j, states[ id ] );

        }
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::drawRect( int x, int y, Node::State state ) {

#define COLOR_CYAN   case Node::BLANK:    ofSetColor( 125, 249, 255 ); break
#define COLOR_RED    case Node::VISITING: ofSetColor( 255,  69,   0 ); break
#define COLOR_GREY   case Node::VISITED:  ofSetColor(  84,  84,  84 ); break
#define COLOR_WHITE  case Node::PATH:     ofSetColor( 255, 255, 255 ); break
#define COLOR_ORANGE case Node::BEGIN:    ofSetColor( 255, 153,  51 ); break
#define COLOR_LIME   case Node::END:      ofSetColor(  50, 205,  50 ); break
#define COLOR_PINK   case Node::PEEKING:  ofSetColor( 246,  83, 166 ); break

    ofSetColor( 253, 213, 177 );    // yellow
    ofRectRounded( x * cubeWidth,
                   y * cubeHeight,
                   cubeWidth,
                   cubeHeight,
                   cubeCornerRadius );

    switch ( state ) {
        COLOR_CYAN;
        COLOR_RED;
        COLOR_GREY;
        COLOR_WHITE;
        COLOR_ORANGE;
        COLOR_LIME;
        COLOR_PINK;
    }

    ofRectRounded( x * cubeWidth  + cubeBorderSize,
                   y * cubeHeight + cubeBorderSize,
                   cubeWidth  - 2.0 * cubeBorderSize,
                   cubeHeight - 2.0 * cubeBorderSize,
                   cubeCornerRadius );

#undef COLOR_CYAN
#undef COLOR_RED
#undef COLOR_GREY
#undef COLOR_WHITE
#undef COLOR_ORANGE
#undef COLOR_LIME
#undef COLOR_PINK

}

void testApp::drawRect( int x, int y, ofColor color ) {

    ofSetColor( 253, 213, 177 );    // yellow
    ofRectRounded( x * cubeWidth,
                   y * cubeHeight,
                   cubeWidth,
                   cubeHeight,
                   cubeCornerRadius );

    ofSetColor( color );
    ofRectRounded( x * cubeWidth  + cubeBorderSize,
                   y * cubeHeight + cubeBorderSize,
                   cubeWidth  - 2.0 * cubeBorderSize,
                   cubeHeight - 2.0 * cubeBorderSize,
                   cubeCornerRadius );


}
