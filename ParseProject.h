#pragma once
//std include
#include <iostream>
#include <fstream>
#include <sstream>
//for JSON
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Stringifier.h>
//for config and result
#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/FileStream.h>
#include <Poco/Net/NetSSL.h>
#include <Poco/Crypto/Crypto.h>
//for multi threading
#include <Poco/Thread.h>
#include <Poco/Mutex.h>
#include <Poco/RunnableAdapter.h>
//for logging
#include <Poco/SplitterChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/SimpleFileChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/Logger.h>
#include <Poco/String.h>
#include <Poco/PatternFormatter.h>
//for connect
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/URI.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/Context.h>





