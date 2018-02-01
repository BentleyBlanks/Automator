#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

struct insurance
{
    // ��ҵ
    float bussinessMoney;

    // �Ƽ�����
    string companyName;
    
    // �����߱���
    float thirdPartyMoney;

    // ��������Լ��
    bool thirdPartyInsurance;

    // ��ȼ��
    bool selfFire;

    // ��������ʧ��
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
    // ҵ��Ա��
    string name;

    // �����߱�������
    int thirdPartyMoney = 0;

    // ��������Լ��
    int thirdPartyInsurance = 0;

    // ��ȼ��
    int selfFire = 0;

    // ��������ʧ��
    int engine = 0; 

    // ����
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
