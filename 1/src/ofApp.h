#pragma once

#include "ofxImGui.h"
#include "ofMain.h"
#include <iostream>  
#include <fstream>  
#include <sstream>  
#include <string>
#include <vector> 
#include <map>

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void print();
    void importPeople(string path);
    void importData(string path);
    void checking();
    void calculating();

    ofxImGui::Gui gui;
    ofImage dog;

    ofstream resultFile;
    ifstream peopleFile, dataFile;
    bool peopleFileLoaded, dataFileLoaded;
    // people-all companys
    map<string, vector<string>*> people;
    // company-data double ptr[9]
    map<string, float*> data;
    // result file data
    // company-data double ptr[9]
    map<string, float*> result;
};
