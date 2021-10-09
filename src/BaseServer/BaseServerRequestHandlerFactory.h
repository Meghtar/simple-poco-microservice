#pragma once

#include <glog/logging.h>
#include <string>

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"

#include "BaseServerRequestHandler.h"
#include "RemoteServerConfig.h"

#include "../DatabaseHandler/DatabaseHandler.h"

class BaseServerRequestHandlerFactory
  : public Poco::Net::HTTPRequestHandlerFactory
{
public:
  BaseServerRequestHandlerFactory(uint16_t remoteServicePort,
                                  const std::string& remoteServiceHost,
                                  DatabaseConfig dbConfig)
    : _remoteServerConfig(
        std::make_unique<RemoteServerConfig>(remoteServiceHost,
                                             remoteServicePort))
    , _dbConfig(dbConfig)
  {
    LOG(INFO) << "created factory with remote: "
              << _remoteServerConfig->getHost() << " on port "
              << _remoteServerConfig->getPort() << " with "
              << _dbConfig._database << " db";
    _database = std::make_shared<DatabaseHandler>(_dbConfig);
  }
  BaseServerRequestHandlerFactory()
    : _remoteServerConfig(std::make_unique<RemoteServerConfig>())
  {}
  Poco::Net::HTTPRequestHandler* createRequestHandler(
    const Poco::Net::HTTPServerRequest& request)
  {
    if (request.getURI() == "/") {
      std::string text = "not using db";
      if (_database) {
        auto documents =
          _database->getAllDocumentsFromCollection("collectionName");
        text =
          "amount of downloaded records: " + std::to_string(documents.size());
      }
      return new BaseServerRequestHandler(text);
    }

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
  DatabaseConfig _dbConfig;
  std::shared_ptr<DatabaseHandler> _database;
};