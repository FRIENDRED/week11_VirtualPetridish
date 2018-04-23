#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(125);
    ballSys.setup(15);
}

//--------------------------------------------------------------
void ofApp::update(){
    ballSys.update();
    
    //only calculate the voronoi if needed (if the mode is > 0)
    if (mode>1){
        voronoi.clear();
        voronoi.setBounds(ofRectangle(-1, -1, ofGetWidth(), ofGetHeight()));
        voronoi.setPoints(ballSys.getPoints());
        voronoi.generate();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (mode==1) {
        ballSys.draw();
        ofDrawBitmapString("Total Agents: " + ofToString(ballSys.balls.size()), 10, 20);
    }
    else drawVoronoi();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    ballSys.infectBall(x,y);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == '1') mode = 1;
    else if (key == '2') mode = 2;
    else if (key == '3') mode = 3;
    else if (key == 'a') ballSys.addBall(mouseX, mouseY);
}

void ofApp::drawVoronoi(){
    ofPushStyle();
    vector <ofxVoronoiCell> cells = voronoi.getCells();
    for(int i=0; i<cells.size(); i++) {
        // DRAW CELL SURFACE
        int value = ballSys.getNearestBall(cells[i].pt).health;
        float pc = ofMap(value,0,255,0,1);
        ofColor c1 = ofColor(255,140,0);
        ofColor c2 = ofColor(0,0,100);
        ofSetColor(c1.getLerped(c2,pc));

        ofFill();
        ofBeginShape();
        for(int j=0; j<cells[i].pts.size(); j++) {
            
            ofPoint thisPt = cells[i].pts[j];
            ofVertex(thisPt);
        }
        ofPoint firstPoint  = cells[i].pts[cells[i].pts.size()-1];
        ofVertex(firstPoint);
        ofEndShape();
        
        // DRAW CELL BORDERS
        if (mode==2){
            ofSetColor(125);
            for(int j=0; j<cells[i].pts.size(); j++) {
                    ofPoint lastPt = cells[i].pts[cells[i].pts.size()-1];
                    if(j > 0) {
                        lastPt = cells[i].pts[j-1];
                    }
                    ofPoint thisPt = cells[i].pts[j];
        
                    if(!isBorder(lastPt) || !isBorder(thisPt)) {
                            ofDrawLine(lastPt, thisPt);
                    }
            }
            //ofSetColor(0);
            //ofDrawCircle(cells[i].pt, 2);
        }
    }
    ofPopStyle();
}

//--------------------------------------------------------------
bool ofApp::isBorder(ofPoint _pt){
    ofRectangle bounds = voronoi.getBounds();
    
    return (_pt.x == bounds.x || _pt.x == bounds.x+bounds.width
            || _pt.y == bounds.y || _pt.y == bounds.y+bounds.height);
}

