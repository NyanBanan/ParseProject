
#include "ParseProject.h"



void connect(){
    std::ifstream conf("paramet312312ers.json", std::ifstream::binary);

    if(!conf.is_open())
        throw std::exception("Config file not open");

    Poco::JSON::Parser parser;
    
    auto pParam=parser.parse(conf).extract<Poco::JSON::Object::Ptr>();
    conf.close();
    std::string key=pParam->getValue<std::string>("key");
    std::string https=pParam->getValue<std::string>("https");
    if(key.empty()){
        std::cout<<"EMPTYYYYY"<<std::endl;
    }
    std::cout<<key<<std::endl;
    std::cout<<https<<std::endl;
}

int main(){

    Poco::AutoPtr<Poco::SimpleFileChannel> pChannel(new Poco::SimpleFileChannel);
    pChannel->setProperty("path", "Work.log");
    pChannel->setProperty("rotation", "2 K");  
    pChannel->setProperty("flush", "true");

   /*  Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(
        new Poco::PatternFormatter("[%Y-%m-%d  %H:%M:%s] [%U(%u)] %p: %t"));
    patternFormatter->setProperty("times", "local");
    Poco::AutoPtr<Poco::FormattingChannel> formattingChannel(
       new Poco::FormattingChannel(patternFormatter, pChannel));

    Poco::Logger::root().setChannel(formattingChannel); */
    Poco::Logger::root().setChannel(pChannel);

    Poco::Logger& logger = Poco::Logger::get("WorkLogger");

    try
    {
        connect();
        logger.information("Work correct");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        logger.error(e.what());
    }

    system("pause");
    pChannel->close();
    return 0;
}

/*  Poco::Path a;
    std::cout<<Poco::Path::find("D:/Diff Folders From C/Workbench/Code/newRepos/ParseProject","parameters.json",a); 
    std::cout<<a.toString();*/