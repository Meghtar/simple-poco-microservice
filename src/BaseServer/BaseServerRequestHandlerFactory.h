#pragma once
#include <string>
#include <iostream>

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Exception.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/JSON/JSON.h"
#include <iostream>

#include <algorithm>
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include <sstream>

#include "BaseServerRequestHandler.h"


using Poco::Net::ServerSocket;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Util::ServerApplication;
using Poco::Util::Application;

using namespace Poco::Net;
using namespace Poco;

class BaseServerRequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
    BaseServerRequestHandlerFactory(uint16_t remoteServicePort, const std::string& remoteServiceHost) : 
    _remoteServicePort(remoteServicePort), _remoteServiceHost(remoteServiceHost) {
        std::cout << "created factory with: " << _remoteServiceHost << " on " << _remoteServicePort << std::endl;
    }
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
    {
        if (request.getURI() == "/")
            return new BaseServerRequestHandler("");
        else if (request.getURI() == "/example")
            return new BaseServerRequestHandler("example", _remoteServiceHost, _remoteServicePort);
        else
            return new BaseServerRequestHandler("Not a default URI");
    }
private:
    const uint16_t _remoteServicePort;
    const std::string _remoteServiceHost;
};