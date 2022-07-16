
#include "ParseProject.h"

void connect(){
    std::ifstream conf("parameters.json", std::ifstream::binary);

    if(!conf.is_open())
        throw Poco::Exception("Config file not open");

    Poco::JSON::Parser parser;
    
    auto pParam=parser.parse(conf).extract<Poco::JSON::Object::Ptr>();
    conf.close();
    std::string key=pParam->getValue<std::string>("key");
    std::string https=pParam->getValue<std::string>("https");
    if(key.empty())
        throw Poco::Exception("Key is empty");

    std::cout<<key<<std::endl;
    std::cout<<https<<std::endl;

    Poco::URI uri(https);
    std::string path=uri.getPathAndQuery();

    if(path.empty())
        throw Poco::Exception("Path empty");

    Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());

    Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET,path);
    req.setCredentials("Bearer ", key);
    req.setContentType("application/json");
    
    session.sendRequest(req);
    
    Poco::Net::HTTPResponse response;

    std::istream &ret = session.receiveResponse(response);
    /* std::stringstream rawJson;
    std::string k;
    Poco::StreamCopier::copyStream(ret, rawJson);
    Poco::StreamCopier::copyToString(ret, k);
    std::cout<<k;
    std::cout<<k; */
    auto result_json=parser.parse(rawJson).extract<Poco::JSON::Object::Ptr>();

    std::cout<<result_json;
}

int main(){

    Poco::AutoPtr<Poco::SimpleFileChannel> pChannel(new Poco::SimpleFileChannel);
    pChannel->setProperty("path", "Work.log");
    pChannel->setProperty("rotation", "2 K");  
    pChannel->setProperty("flush", "true");

    Poco::Logger::root().setChannel(pChannel);

    Poco::Logger& logger = Poco::Logger::get("WorkLogger");

    try
    {
        connect();
        logger.information("Work correct");
    }
    catch(const Poco::Exception& e){
        logger.error(e.message());
        return -1;
    }

    catch(const std::exception& e){
        logger.error(e.what());
        return -1;
    }
    
    system("pause");
    pChannel->close();
    return 0;
}

/*  Poco::Path a;
    std::cout<<Poco::Path::find("D:/Diff Folders From C/Workbench/Code/newRepos/ParseProject","parameters.json",a); 
    std::cout<<a.toString();*/
    
   /*  Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(
        new Poco::PatternFormatter("[%Y-%m-%d  %H:%M:%s] [%U(%u)] %p: %t"));
    patternFormatter->setProperty("times", "local");
    Poco::AutoPtr<Poco::FormattingChannel> formattingChannel(
       new Poco::FormattingChannel(patternFormatter, pChannel));

    Poco::Logger::root().setChannel(formattingChannel); */