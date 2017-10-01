#include "ofApp.h"
static int styleID = 0;
#define IMGUI_STYLE_BEGIN(style)    static int tempStyleID = styleID++;\
                                    ImGui::PushID(tempStyleID);\
                                    ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(style / 7.0f, 0.6f, 0.6f));\
                                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(style / 7.0f, 0.7f, 0.7f));\
                                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(style / 7.0f, 0.8f, 0.8f));

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

inline float percision(float number)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << number;
    ss >> number;
    return number;
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Automator");
    dog.loadImage("1.jpg");
    gui.setup();

    peopleFileLoaded = false;
    dataFileLoaded = false;
}

//--------------------------------------------------------------
void ofApp::print()
{
    // test
    map<string, vector<string>*>::iterator iter;
    for(iter = people.begin(); iter != people.end(); iter++)
    {
        cout << "| " << iter->first;
        vector<string>* stringPtr = iter->second;

        for(int i = 0; i < stringPtr->size(); i++)
            cout << (*stringPtr)[i] << ",";

        cout << " |" << endl;
    }

    map<string, float*>::iterator it;
    for(it = data.begin(); it != data.end(); it++)
    {
        cout << "| " << it->first << endl;
        for(int i = 0; i < 9; i++)
        {
            cout << (it->second)[i] << ",";
        }
        cout << endl;
    }
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
    while(getline(peopleFile, line))
    {
        //cout << "people primitive£º" << line << endl;
        istringstream sin(line);

        string companyName, name;
        // , for seprating
        getline(sin, companyName, ',');
        getline(sin, name, ',');

        //cout << "xx:" << companyName << "," << name << endl;

        // if the name not exsited
        map<string, vector<string>*>::iterator it = people.find(name);
        if(it == people.end())
        {
            // init the company name string vector
            vector<string>* stringPtr = new vector<string>();
            stringPtr->push_back(companyName);

            people[trim(name)] = stringPtr;
        }
        else
        {
            // add in company name list
            people[trim(name)]->push_back(trim(companyName));
        }
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
    // data
    // skip 2 of it
    getline(dataFile, line);
    getline(dataFile, line);
    while(getline(dataFile, line))
    {
        //cout << "data primitive£º" << line << endl;
        istringstream sin(line);

        string companyName;
        getline(sin, companyName, ',');
        trim(companyName);

        float* dataPtr = new float[9];

        string tempString;
        for(int i = 0; i < 9; i++)
        {
            getline(sin, tempString, ',');
            trim(tempString);

            if(i != 2 || i != 5 || i != 8)
                dataPtr[i] = ofToFloat(tempString);
            else
                // erase the % symbol
                dataPtr[i] = ofToFloat(trimPercent(tempString));
        }

        data[companyName] = dataPtr;
    }
}

//--------------------------------------------------------------
void ofApp::checking()
{    
    // Checking company with people
    bool allFinded = true;
    map<string, float*>::iterator iter;
    for(iter = data.begin(); iter != data.end(); iter++)
    {
        string companyName = iter->first;

        // find in people's map
        bool finded = false;
        for(map<string, vector<string>*>::iterator it = people.begin(); it != people.end(); it++)
        {
            // loop the person's all company
            vector<string>* stringPtr = it->second;
            for(int i = 0; i < stringPtr->size(); i++)
            {
                if((*stringPtr)[i] == companyName)
                {
                    finded = true;
                    break;
                }
            }
        }

        if(!finded)
        {
            cout << "Company:" << companyName << " Not find!" << endl;
            finded = false;
            allFinded = false;
        }
    }

    if(allFinded)
        cout << "Successful matched in 2 files!" << endl;
}

//--------------------------------------------------------------
void ofApp::calculating()
{
    string path = "./data/result.csv";
    resultFile.open(path.c_str());
    for(map<string, vector<string>*>::iterator it = people.begin(); it != people.end(); it++)
    {
        string name = it->first;
        // for summing up the person's data
        float sumData[10] = {0};

        // loop the person's all company
        vector<string>* stringPtr = it->second;
        for(int i = 0; i < stringPtr->size(); i++)
        {
            string companyName = (*stringPtr)[i];

            // find in data map
            map<string, float*>::iterator it = data.find(companyName);
            if(it != data.end())
            {
                float *originData = it->second;
                float resultData[10];

                // data generating
                resultData[0] = percision(originData[0]);
                resultData[1] = percision(originData[1]);
                resultData[2] = percision(resultData[0] - resultData[1]);

                resultData[3] = percision(originData[3] + originData[6]);
                resultData[4] = percision(originData[4] + originData[7]);
                resultData[5] = percision(resultData[3] - resultData[4]);

                resultData[6] = percision(resultData[0] + resultData[3]);
                resultData[7] = percision(resultData[1] + resultData[4]);
                resultData[8] = percision(resultData[6] - resultData[7]);
                // per cent %
                resultData[9] = percision((resultData[8] / resultData[7]) * 100.0f);

                // saving to file
                resultFile << name << ",";
                resultFile << companyName << ",";
                for(int i = 0; i < 10; i++)
                {
                    if(i != 9)
                        resultFile << resultData[i] << ",";
                    else
                        resultFile << resultData[i] << "%";
                }

                resultFile << endl;

                // add the company to sumData(except for the last one)
                for(int i = 0; i < 9; i++)
                    sumData[i] += resultData[i];
            }
        }

        // summing up
        resultFile << "ºÏ¼Æ,,";

        sumData[9] = percision((sumData[8] / sumData[7]) * 100.0f);
        for(int i = 0; i < 10; i++)
        {
            if(i != 9)
                resultFile << percision(sumData[i]) << ",";
            else
                resultFile << percision(sumData[i]) << "%";
        }
        resultFile << endl;
        // next line
        resultFile << ",,,,,,,,,,," << endl;
    }
    resultFile.close();
    printf("File already saved to :%s", path.c_str());
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
    window_flags |= ImGuiWindowFlags_NoResize;

    ImGui::Begin("", &open, ImVec2(), 1.0f, window_flags);
    ImVec2 rect = ImVec2(300, 60);
    
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
        if(ImGui::Button("Checking", rect))
        {
            if(peopleFileLoaded && dataFileLoaded)
                checking();
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
    ImGui::End();
    gui.end();

    ofDrawBitmapString("Ver 1.2 By BB", 570, 650);
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
