
#include "ParseProject.h"
//all processes running in constructor of Responcer class
class Responcer
{
private:
    std::tuple<std::string,std::string> key_and_adress; //Token to connect and URI adress
    Poco::SharedPtr<Poco::Net::HTTPSClientSession> session;//Pointer to connect session so as not to create it at every request
    Poco::SharedPtr<Poco::Net::HTTPRequest> req; //Like session pointer, but for request
    std::string path; 
    Poco::Mutex mutex;
    Poco::Logger& logger=Poco::Logger::root();
    std::tuple<std::string,std::string> getParameters(std::string config_path); //initilizer of Token and URI
    void initilizeLogger(); //initilizer for logger
    void startSession(); 
    void logging();//process of logging
    void getResponce();
    
public:
    Responcer(std::string config_path);
    ~Responcer();
};

Responcer::Responcer(std::string config_path)
{
    initilizeLogger();
    key_and_adress=getParameters(config_path);
    startSession();

    Poco::RunnableAdapter<Responcer> runnable (*this, &Responcer::logging);
    
    Poco::Thread log_thread;
    log_thread.start(runnable);
    getResponce();
    system("pause");
}

void Responcer::initilizeLogger(){
    try{
    //file logger
    Poco::AutoPtr<Poco::SimpleFileChannel> pFile(new Poco::SimpleFileChannel);
    pFile->setProperty("path", "Work.log");
    pFile->setProperty("rotation", "2 M");  
    pFile->setProperty("flush", "true");
    //console logger
    Poco::AutoPtr<Poco::ConsoleChannel> pCons(new Poco::ConsoleChannel);
    //synchronize loggers
    Poco::AutoPtr<Poco::SplitterChannel> splitter_Channel(new Poco::SplitterChannel);
    splitter_Channel->addChannel(pFile);
    splitter_Channel->addChannel(pCons);
    //formatter
    Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(
       new Poco::PatternFormatter("[%Y-%m-%d  %H:%M] %p: %t"));
    patternFormatter->setProperty("times", "local");
    Poco::AutoPtr<Poco::FormattingChannel> formattingChannel(
       new Poco::FormattingChannel(patternFormatter, splitter_Channel));
       
    logger.setChannel(formattingChannel);
    }
    catch(Poco::Exception& e){
        logger.error(std::string("(In InitilizeLogger) ")+e.name()+e.message());
    }
    catch(std::exception& e){
        logger.error(std::string("(In InitilizeLogger) ")+e.what());
    }
}

void Responcer::logging(){
    while(true){
    try{
    Poco::ScopedLock lock(mutex);

    session->sendRequest(*req);
    
    Poco::Net::HTTPResponse response;
    session->receiveResponse(response);
    logger.information(response.getReason());
    std::stringstream ss;
    response.write(ss);
    std::string temp=ss.str();
    temp = Poco::replace(temp,"\n\n","");
    temp = Poco::replace(temp,"\r\n\r\n","");
    temp = Poco::replace(temp,"\r\n","\n");
    logger.information(temp);
    Poco::Thread::yield();
    }
    catch(Poco::Exception& e){
        logger.error(std::string("(In logging) ")+e.name()+e.message());
    }
    catch(std::exception& e){
        logger.error(std::string("(In logging) ")+e.what());
    }
    Poco::Thread::sleep(10000);
    }
}

Responcer::~Responcer()
{
    logger.information("End of work");
    logger.close();
}

void Responcer::startSession(){

    Poco::URI uri(std::get<1>(this->key_and_adress));
    path=uri.getPathAndQuery();
    try{
    if(path.empty())
        path="/";
    Poco::Net::initializeSSL();
    const Poco::Net::Context::Ptr context = new Poco::Net::Context(
            Poco::Net::Context::TLS_CLIENT_USE, "", "", "",
            Poco::Net::Context::VERIFY_NONE, 9, false,
            "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    session = new Poco::Net::HTTPSClientSession(uri.getHost(),uri.getPort(),context);
    req=new Poco::Net::HTTPRequest(Poco::Net::HTTPRequest::HTTP_GET,path,Poco::Net::HTTPMessage::HTTP_1_1);
    req->setCredentials("Token",std::get<0>(this->key_and_adress));
    req->setContentType("application/json");
    }
    catch(Poco::Exception& e){
        logger.error(std::string("(In startSession) ")+e.name()+e.message());
    }
    catch(std::exception& e){
        logger.error(std::string("(In startSession) ")+e.what());
    }
}

std::tuple<std::string,std::string> Responcer::getParameters(std::string file_path){
    Poco::FileStream conf;
    try{
        Poco::File check(file_path);
        if(check.exists())
            conf.open(file_path, std::ios::in);
        else 
            throw Poco::Exception("");
    }
    catch(Poco::FileException& e){
        logger.error(std::string("(In getParameters) Config file not open ")+e.name()+" "+e.message());
        return std::make_tuple(nullptr,nullptr);
    }
    catch(std::exception& e){
        logger.error(std::string("(In getParameters) Config file not open ")+e.what());
        return std::make_tuple(nullptr,nullptr);
    }
    Poco::JSON::Object::Ptr pParam;
    Poco::JSON::Parser parser;
    try{
        pParam = parser.parse(conf).extract<Poco::JSON::Object::Ptr>();
    }
    catch(Poco::Exception& e){
        logger.error(std::string("(In getParameters) Config file error ")+e.name()+" "+e.message());
        return std::make_tuple(nullptr,nullptr);
    }
    catch(std::exception& e){
        logger.error(std::string("(In getParameters) Config file error ")+e.what());
        return std::make_tuple(nullptr,nullptr);
    }
    conf.close();
    std::string key=pParam->getValue<std::string>("key");
    std::string https=pParam->getValue<std::string>("https");
    return std::make_tuple(key,https);
}

void Responcer::getResponce(){
    try{
    
    Poco::ScopedLock lock(mutex);

    session->sendRequest(*req);
    
    Poco::Net::HTTPResponse response;
    std::istream &ret = session->receiveResponse(response);

    Poco::JSON::Parser parser;

    Poco::Dynamic::Var parsed_json=parser.parse(ret);

    Poco::FileStream conf;
    conf.open("Answer.json",std::ios::trunc);
    
    Poco::JSON::Stringifier::stringify(parsed_json,conf);
    }
    catch(Poco::Exception& e){
        logger.error(std::string("(In getResponce) ")+e.message());
    }
    catch(std::exception& e){
        logger.error(std::string("(In getResponce) ")+e.what());
    }
}

int wmain(){
    Responcer a("parameters.json");
    return 0;
}