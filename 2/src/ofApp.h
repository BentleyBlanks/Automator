#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

struct insurance
{
    // 商业
    float bussinessMoney;

    // 推荐送修
    string companyName;
    
    // 第三者保额
    float thirdPartyMoney;

    // 第三方特约险
    bool thirdPartyInsurance;

    // 自燃险
    bool selfFire;

    // 发动机损失险
    bool engine;

    void print()
    {
        printf("%f, %s, %f, %d, %d, %d\n", bussinessMoney, companyName.c_str(), thirdPartyMoney, thirdPartyInsurance, selfFire, engine);
    }
};

struct people
{
    string name;
    string companyName;    
    
    void print()
    {
        printf("%s, %s\n", name.c_str(), companyName.c_str());
    }
};

struct clerk
{
    // 业务员名
    string name;

    // 第三者保额数量
    int thirdPartyMoney = 0;

    // 第三方特约险
    int thirdPartyInsurance = 0;

    // 自燃险
    int selfFire = 0;

    // 发动机损失险
    int engine = 0; 

    // 保单
    int quantity = 0;

    void print()
    {
        printf("%s, %d, %d, %d, %d, %d\n", name.c_str(), thirdPartyMoney, thirdPartyInsurance, selfFire, engine, quantity);
    }
};

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
    void calculating();
    void saving();

    ofxImGui::Gui gui;
    ofImage dog;

    ofstream resultFile;
    ifstream peopleFile, dataFile;
    bool peopleFileLoaded, dataFileLoaded;
    // people-all companys
    vector<people> staff;
    vector<insurance> data;
    // result file data
    vector<clerk*> result;

    float percent;
};
