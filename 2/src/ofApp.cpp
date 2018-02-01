#include "ofApp.h"
static int styleID = 0;
#define IMGUI_STYLE_BEGIN(style)    static int tempStyleID = styleID++;\
                                    ImGui::PushID(tempStyleID);\
                                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(style / 7.0f, 0.6f, 0.6f));\
                                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(style / 7.0f, 0.7f, 0.7f));\
                                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(style / 7.0f, 0.8f, 0.8f));

#define IMGUI_STYLE_END()           ImGui::PopStyleColor(3); \
                                    ImGui::PopID();

inline std::string trim(std::string& str)
{
    str.erase(0, str.find_first_not_of(" \t\r\n"));
    str.erase(str.find_last_not_of(" \t\r\n") + 1);
    return str;
}

inline std::string trimPercent(std::string& str)
{
    str.erase(0, str.find_first_not_of("%"));
    str.erase(str.find_last_not_of("%") + 1);
    return str;
}

//--------------------------------------------------------------
void ofApp::setup(){
    dog.load("doge.jpg");
    gui.setup();

    peopleFileLoaded = false;
    dataFileLoaded = false;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    dog.draw(0, 0);
    gui.begin();

    static bool open = true;
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    //window_flags |= ImGuiWindowFlags_NoResize;

    ImGui::Begin("", &open, ImVec2(), 1.0f, window_flags);
    ImVec2 rect = ImVec2(250, 50);

    if(ImGui::Button("People File Import", rect))
    {
        ofFileDialogResult result = ofSystemLoadDialog("People File Select");
        if(result.bSuccess)
        {
            peopleFileLoaded = true;
            importPeople(result.filePath);
        }
    }

    ImGui::SameLine();
    if(ImGui::Button("Data File Import", rect))
    {
        ofFileDialogResult result = ofSystemLoadDialog("Data File Select");
        if(result.bSuccess)
        {
            dataFileLoaded = true;
            importData(result.filePath);
        }
    }

    ImGui::Separator();

    {
        IMGUI_STYLE_BEGIN(4);
        if(ImGui::Button("Saving", rect))
        {
            if(peopleFileLoaded || dataFileLoaded)
            {
                print();
                saving();
            }
            else
                printf("Please import files in it\n");
        }
        IMGUI_STYLE_END();
    }

    ImGui::SameLine();
    {
        IMGUI_STYLE_BEGIN(2);
        if(ImGui::Button("Calculating", rect))
        {
            if(peopleFileLoaded && dataFileLoaded)
                calculating();
            else
                printf("Please import files in it\n");
        }
        IMGUI_STYLE_END();
    }

    //ImGui::Separator();
    //ImGui::Text("Calculating");
    //ImGui::SameLine();
    //ImGui::ProgressBar(percent, ImVec2(-1, 0));
    //ImGui::DragFloat("Percent", &percent, 1.0f, 0.0f, 0.0f, "%.2f");

    ImGui::End();
    gui.end();

    ofDrawBitmapString("Ver 1.0 By BB", 500, 570);
}

//--------------------------------------------------------------
void ofApp::print()
{
    for(int i = 0; i < 20; i++)
        data[i].print();

    printf("\n");

    for(auto s : staff)
        s.print();

    printf("\n");

    for(auto r : result)
        r->print();
}

//--------------------------------------------------------------
void ofApp::importPeople(string path)
{
    peopleFile.open(path.c_str());
    if(peopleFile.fail())
        cerr << "Error: " << strerror(errno);
    else
        printf("File:%s loaded successfully.\n", path.c_str());

    // people
    string line; 
    // skip one of it
    people p;
    getline(peopleFile, line);
    while(getline(peopleFile, line))
    {
        istringstream sin(line);

        string temp;
        getline(sin, temp, ',');

        // get [b]'s data
        string companyName;
        getline(sin, companyName, ',');
        trim(companyName);
        p.companyName = companyName;

        getline(sin, temp, ',');

        // get [D]'s data
        string name;
        getline(sin, name, ',');
        trim(name);
        p.name = name;

        staff.push_back(p);
    }
}

//--------------------------------------------------------------
void ofApp::importData(string path)
{
    dataFile.open(path.c_str());
    if(dataFile.fail())
        cerr << "Error: " << strerror(errno);
    else
        printf("File:%s loaded successfully.\n", path.c_str());

    string line;
    istringstream sin(line);
    
    insurance ins;
    // data
    // skip one of it
    getline(dataFile, line);
    while(getline(dataFile, line))
    {
        //cout << "data primitive：" << line << endl;
        istringstream sin(line);

        // to [G]
        string temp;
        for(int i = 0; i < 6; i++)
            getline(sin, temp, ',');

        // get the [G]'s data(商业)
        string bussinessQuantity;
        getline(sin, bussinessQuantity, ',');
        trim(bussinessQuantity);
        ins.bussinessMoney = ofToFloat(bussinessQuantity);

        // to [L]
        for(int i = 0; i < 4; i++)
            getline(sin, temp, ',');

        // get the [L]'s data(推荐送修)
        getline(sin, ins.companyName, ',');
        trim(ins.companyName);

        // to [N]
        getline(sin, temp, ',');

        // get the [N]'s data(第三者保额)
        string money;
        getline(sin, money, ',');
        trim(money);
        ins.thirdPartyMoney = ofToFloat(money);

        // to [V]
        for(int i = 0; i < 7; i++)
            getline(sin, temp, ',');

        // get [v]'s data(第三方特约险)
        string thirdParty;
        getline(sin, thirdParty, ',');
        trim(thirdParty);
        if(thirdParty == "")
            ins.thirdPartyInsurance = false;
        else
            ins.thirdPartyInsurance = true;

        // get [W]'s data(第三方特约险)
        string selfFire;
        getline(sin, selfFire, ',');
        trim(selfFire);
        if(selfFire == "")
            ins.selfFire = false;
        else
            ins.selfFire = true;

        // get [X]'s data(发动机损失险)
        string engine;
        getline(sin, engine, ',');
        trim(engine);
        if(engine == "")
            ins.engine = false;
        else
            ins.engine = true;

        data.push_back(ins);
    }
}

//--------------------------------------------------------------
void ofApp::calculating()
{
    for(int i = 0; i < data.size(); i++)
    {
        printf("Calculating: %.2f %% \r", 100.0f * i / (float) data.size());
        percent = i / (float) data.size();

        insurance& ins = data[i];

        string name;
        // get the clerk's name
        for(auto p : staff)
        {
            if(ins.companyName == p.companyName)
            {
                name = p.name;
                break;
            }
        }

        if(name == "")
            continue;

        clerk* c = NULL;

        // through the company's name to find the staff
        for(auto r : result)
        {
            if(r->name == name)
                c = r;
        }

        if(!c)
        {
            // not find: add a clerk
            c = new clerk();
            c->name = name;
            result.push_back(c);
        }

        if(ins.bussinessMoney > 0.0f)
            c->quantity++;

        if(ins.engine)
            c->engine++;

        if(ins.selfFire)
            c->selfFire++;

        if(ins.thirdPartyInsurance)
            c->thirdPartyInsurance++;

        if(ins.thirdPartyMoney > 2000000)
            c->thirdPartyMoney++;
    }
}

void ofApp::saving()
{
    if(result.size() <= 0)
        printf("Import data, people file first.");

    string path = "./data/result.csv";
    resultFile.open(path.c_str());

    resultFile << ",第三方保额,第三方特约险,自燃险,发动机损失险,保单数" << endl;
    for(auto r : result)
    {
        resultFile << r->name << ",";
        resultFile << r->thirdPartyMoney << ",";
        resultFile << r->thirdPartyInsurance << ",";
        resultFile << r->selfFire << ",";
        resultFile << r->engine << ",";
        resultFile << r->quantity << endl;
    }
    resultFile.close();
    printf("File already saved to :%s", path.c_str());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
