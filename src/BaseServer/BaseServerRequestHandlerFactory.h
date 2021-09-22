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
#include "RemoteServerConfig.h"


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
    _remoteServerConfig(std::make_unique<RemoteServerConfig>(remoteServiceHost, remoteServicePort))
    {
        std::cout << "created factory with remote: " <<  _remoteServerConfig->getHost() << " on port " <<  _remoteServerConfig->getPort() << std::endl;
    }
    BaseServerRequestHandlerFactory() :
    _remoteServerConfig(std::make_unique<RemoteServerConfig>())
    {}
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
    {
        if (request.getURI() == "/")
            return new BaseServerRequestHandler("");
        else if (request.getURI() == "/remote")
        {
            if (_remoteServerConfig->useRemote())
                return new BaseServerRequestHandler("remote", _remoteServerConfig->getHost(), _remoteServerConfig->getPort());
            else
                return new BaseServerRequestHandler("remote but turned off");
        }
        else
            return new BaseServerRequestHandler("Not a default URI");
    }
private:
    const std::unique_ptr<RemoteServerConfig> _remoteServerConfig;
};