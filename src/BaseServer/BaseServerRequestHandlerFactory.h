#pragma once

#include <glog/logging.h>
#include <string>

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"

#include "BaseServerRequestHandler.h"
#include "RemoteServerConfig.h"

class BaseServerRequestHandlerFactory
  : public Poco::Net::HTTPRequestHandlerFactory
{
public:
  BaseServerRequestHandlerFactory(uint16_t remoteServicePort,
                                  const std::string& remoteServiceHost)
    : _remoteServerConfig(
        std::make_unique<RemoteServerConfig>(remoteServiceHost,
                                             remoteServicePort))
  {
    LOG(INFO) << "created factory with remote: "
              << _remoteServerConfig->getHost() << " on port "
              << _remoteServerConfig->getPort();
  }
  BaseServerRequestHandlerFactory()
    : _remoteServerConfig(std::make_unique<RemoteServerConfig>())
  {}
  Poco::Net::HTTPRequestHandler* createRequestHandler(
    const Poco::Net::HTTPServerRequest& request)
  {
    if (request.getURI() == "/")
      return new BaseServerRequestHandler("");
    else if (request.getURI() == "/remote") {
      if (_remoteServerConfig->useRemote())
        return new BaseServerRequestHandler("remote",
                                            _remoteServerConfig->getHost(),
                                            _remoteServerConfig->getPort());
      else
        return new BaseServerRequestHandler("remote but turned off");
    } else
      return new BaseServerRequestHandler("Not a default URI");
  }

private:
  const std::unique_ptr<RemoteServerConfig> _remoteServerConfig;
};