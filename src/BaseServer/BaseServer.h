#pragma once
#include <string>

#include "Poco/Net/HTTPServer.h"
#include "Poco/Util/ServerApplication.h"

#include "BaseServerRequestHandlerFactory.h"
#include "BaseServerRequestHandler.h"


class BaseServer: public Poco::Util::ServerApplication
{
protected:
    int main(const std::vector<std::string>& args);
    void handleConfig(const std::string& name, const std::string& value)
    {
        config().setString(name, value);
    }
    int loadConfig();
};