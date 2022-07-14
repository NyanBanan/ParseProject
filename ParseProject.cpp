
#include "ParseProject.h"

bool getConfig(std::string& file_for_conf){
    std::ifstream conf("config.json", std::ifstream::binary);
    if(!conf.is_open())
        return false;
    
}


bool connect(){
    std::ifstream conf("parameters.json", std::ifstream::binary);
    if(!conf.is_open())
        return false;
    Poco::JSON::Parser parser;
    
    auto pParam=parser.parse(conf).extract<Poco::JSON::Object::Ptr>();
    std::string key=pParam->getValue<std::string>("key");
    std::string https=pParam->getValue<std::string>("https");
    if(key.empty()){
        std::cout<<"EMPTYYYYY"<<std::endl;

    }
    std::cout<<key;
    std::cout<<https;
}

int main(){
    std::string fakeJson="{ \"key\" : \"\", \"https\":\"https://github.com/\"}";
    connect();
    system("pause");
    return 0;
}