#pragma once

#include "ofMain.h"
#include "graph.h"
#include <vector>

typedef vector< ofColor > ColorVector;

class testApp : public ofBaseApp{
public:
    ~testApp();

    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

private:
    void drawRect( int x, int y, Node::State state );
    void drawRect( int x, int y, ofColor color );

    int numCubeX;
    int numCubeY;

    int windowWidth;
    int windowHeight;

    float cubeWidth;
    float cubeHeight;
    float cubeBorderSize;
    float cubeCornerRadius;

    Graph       *graph;
    GraphThread *thread;

};
