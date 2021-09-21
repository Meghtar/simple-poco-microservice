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

class BaseServerRequestHandler: public Poco::Net::HTTPRequestHandler
{
public:
    BaseServerRequestHandler(const std::string& text): _text(text) {}
    BaseServerRequestHandler(const std::string& text, const std::string& host, uint16_t port): _text(text), _remoteHost(host), _remotePort(port), _useRemote(true) {}


    std::string getRequestContent(Poco::Net::HTTPServerRequest& request);
    std::string prepareResponse(const Poco::Net::HTTPServerRequest& request, const std::string& additionalText);

    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

    std::string sendRequest(std::string url, std::string body, std::map<std::string,std::string> headers) const;
private:
    std::string _text;
    std::string _remoteHost = "localhost";
    uint16_t _remotePort = 0;
    bool _useRemote = false;

    std::string createUrl(const std::string& protocol, const std::string& host, const uint16_t port, const std::string& endpoint = "");
};