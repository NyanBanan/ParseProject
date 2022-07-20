#include "Responcer.h"
#include "FunctionErrorsVariables.h"

std::string InGetParametersConfigNotOpen{"(In getParameters) Config file not open "};
std::string InGetParametersConfigError{"(In getParameters) Config file error "};
std::string InInitilizeLogger{"(In InitilizeLogger) "};
std::string InLogging{"(In logging) "};
std::string InStartSession{"(In startSession) "};
std::string InGetResponce{"(In getResponce) "};

Responcer::Responcer(const std::string& config_path){
    initilizeLogger();
    setParameters(config_path);
    startSession(this->key_address);

    Poco::RunnableAdapter<Responcer> runnable (*this, &Responcer::logging);
    
    Poco::Thread log_thread;
    log_thread.start(runnable);
    writeJsonAnswer();
    system("pause");
}

Responcer::~Responcer(){
    logger.information("End of work");
    logger.close();
}

void Responcer::initilizeLogger(){
    try{
        //file logger
        Poco::SimpleFileChannel::Ptr pFile(std::make_shared<Poco::SimpleFileChannel>().get());
        pFile->setProperty("path", "Work.log");
        pFile->setProperty("rotation", "2 M");
        pFile->setProperty("flush", "true");
        //console logger
        Poco::ConsoleChannel::Ptr pCons(std::make_shared<Poco::ConsoleChannel>().get());
        //synchronize loggers
        Poco::SplitterChannel::Ptr splitter_Channel(std::make_shared<Poco::SplitterChannel>().get());
        splitter_Channel->addChannel(pFile);
        splitter_Channel->addChannel(pCons);
        //formatter
        Poco::PatternFormatter::Ptr patternFormatter(
            std::make_shared<Poco::PatternFormatter>("[%Y-%m-%d  %H:%M] %p: %t").get());
        patternFormatter->setProperty("times", "local");
        Poco::FormattingChannel::Ptr formattingChannel(
            std::make_shared<Poco::FormattingChannel>(patternFormatter, splitter_Channel).get());
       
        logger.setChannel(formattingChannel);
    }
    catch(Poco::Exception& e){
        logger.error(InInitilizeLogger+e.name()+e.message());
    }
    catch(std::exception& e){
        logger.error(InInitilizeLogger+e.what());
    }
}

void Responcer::logging(){
    while(true){
        try{
            Poco::Net::HTTPResponse response;
            std::stringstream ss;
            std::string temp;

            Poco::ScopedLock lock(mutex);

            session->sendRequest(*req);
    
            session->receiveResponse(response);
            logger.information(response.getReason());
    
            response.write(ss);
    
            temp=ss.str();
            temp = Poco::replace(temp,"\n\n","");
            temp = Poco::replace(temp,"\r\n\r\n","");
            temp = Poco::replace(temp,"\r\n","\n");
            logger.information(temp);
            Poco::Thread::yield();
        }
        catch(Poco::Exception& e){
            logger.error(InLogging+e.name()+e.message());
        }
        catch(std::exception& e){
            logger.error(InLogging+e.what());
        }
        Poco::Thread::sleep(10000);
    }
}

void Responcer::startSession(const ConnectData& key_address){

    Poco::URI uri(key_address.address);
    path=uri.getPathAndQuery();
    //const Poco::Net::Context::Ptr context;

    try{
        if(path.empty())
            path="/";
        Poco::Net::initializeSSL();
        const Poco::Net::Context::Ptr context = std::make_shared<Poco::Net::Context>(
            Poco::Net::Context::TLS_CLIENT_USE, "", "", "",
            Poco::Net::Context::VERIFY_NONE, 9, false,
            "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH").get();
        session = std::make_shared<Poco::Net::HTTPSClientSession>(uri.getHost(),uri.getPort(),context);
        req= new Poco::Net::HTTPRequest(Poco::Net::HTTPRequest::HTTP_GET,path,Poco::Net::HTTPMessage::HTTP_1_1);
        req->setCredentials("Token",key_address.key);
        req->setContentType("application/json");
    }
    catch(Poco::Exception& e){
        logger.error(InStartSession+e.name()+e.message());
    }
    catch(std::exception& e){
        logger.error(InStartSession+e.what());
    }
}

void Responcer::setParameters(const std::string& file_path){
    Poco::FileStream conf;
    Poco::File check(file_path);
    Poco::JSON::Object::Ptr pParam;
    Poco::JSON::Parser parser;

    try{
        if(check.exists())
            conf.open(file_path, std::ios::in);
        else 
            throw Poco::Exception("");
    }
    catch(Poco::FileException& e){
        logger.error(InGetParametersConfigNotOpen+e.name()+" "+e.message());
        return;
    }
    catch(std::exception& e){
        logger.error(InGetParametersConfigNotOpen+e.what());
        return;
    }

    try{
        pParam = parser.parse(conf).extract<Poco::JSON::Object::Ptr>();
    }
    catch(Poco::Exception& e){
        logger.error(InGetParametersConfigError+e.name()+" "+e.message());
        return;
    }
    catch(std::exception& e){
        logger.error(InGetParametersConfigError+e.what());
        return;
    }
    conf.close();
    key_address.key=pParam->getValue<std::string>("key");
    key_address.address=pParam->getValue<std::string>("https");
    
}

void Responcer::writeJsonAnswer(){
    
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var parsed_json;
    Poco::Net::HTTPResponse response;
    Poco::FileStream conf;
    try{
        Poco::ScopedLock lock(mutex);

        session->sendRequest(*req);
    
        std::istream &ret = session->receiveResponse(response);

        parsed_json=parser.parse(ret);

        conf.open("Answer.json",std::ios::trunc);
    
        Poco::JSON::Stringifier::stringify(parsed_json,conf);
    }
    catch(Poco::Exception& e){
        logger.error(InGetResponce+e.message());
    }
    catch(std::exception& e){
        logger.error(InGetResponce+e.what());
    }
}