
#include "ParseProject.h"

void printArray(Poco::JSON::Array::Ptr array);

void printObject(Poco::JSON::Object::Ptr object){
    //print the object
    Poco::JSON::Object::ConstIterator it = object->begin();
    Poco::JSON::Object::ConstIterator endit = object->end();
    while(it != endit){
        std::cout << it->first << " : ";
        if(it->second.isNumeric()){
            int val=0;
            it->second.convert<int>(val);
            std::cout << val;
        }else if(it->second.isString()){
            std::string val;
            it->second.convert<std::string>(val);
            std::cout << val;
        }else if(it->second.isBoolean()){
            bool val=0;
            it->second.convert<bool>(val);
            std::cout << val;
        }else if(it->second.isEmpty()){
            std::cout << "null";
        }else {
            //array
            try{
                Poco::JSON::Array::Ptr spArray;
                spArray = it->second.extract<Poco::JSON::Array::Ptr>();
                printArray(spArray);
                ++it;
                continue;
            }
            catch(Poco::BadCastException&){/*continue silently*/}
            //object
            try{
                Poco::JSON::Object::Ptr spObject;
                spObject = it->second.extract<Poco::JSON::Object::Ptr>();
                printObject(spObject);
                ++it;
                continue;
            }
            catch(Poco::BadCastException&){/*continue silently*/}
        }
        std::cout << std::endl;
        ++it;
    }
}

void printArray(Poco::JSON::Array::Ptr array){
    for(std::size_t i=0; i<array->size(); ++i){
        Poco::JSON::Object::Ptr spObj = array->getObject(i);

        printObject(spObj);
    }
}

void printJson(Poco::Dynamic::Var obj){
    if(obj.isArray()){
        auto result = obj.extract<Poco::JSON::Array::Ptr>();
        printArray(result);
    }
    else{
        auto result = obj.extract<Poco::JSON::Object::Ptr>();
        printObject(result);
    }
}

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

    Poco::URI uri(https);
    std::string path=uri.getPathAndQuery();

    if(path.empty())
        path="/";
    Poco::Net::initializeSSL();
    const Poco::Net::Context::Ptr context = new Poco::Net::Context(
            Poco::Net::Context::CLIENT_USE, "", "", "",
            Poco::Net::Context::VERIFY_NONE, 9, false,
            "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");

    Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(),context);

    Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);

    req.setCredentials("Token",key);
    req.setContentType("application/json");
    req.write(std::cout);

    session.sendRequest(req);
    
    Poco::Net::HTTPResponse response;
    std::istream &ret = session.receiveResponse(response);
    std::cout << response.getStatus() << " " << response.getReason() << std::endl;
    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED) 
        throw Poco::Exception("Authorization Error");
    response.write(std::cout);
    std::cout<< std::endl;
    Poco::Dynamic::Var parsed_json=parser.parse(ret);

    printJson(parsed_json);
    //std::cout<<(*result_json).get("message").toString();
}


int wmain(){

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
        std::cerr<<e.message();
        logger.error(e.message());
        return -1;
    }

    catch(const std::exception& e){
        std::cerr<<e.what();
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
        /* req.setMethod(Poco::Net::HTTPRequest::HTTP_GET);
    req.setURI(path);
    req.setVersion(Poco::Net::HTTPMessage::HTTP_1_1); */