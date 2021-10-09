#include "BaseServer.h"

#include <memory>

#include "Poco/AutoPtr.h"
#include "Poco/Util/IniFileConfiguration.h"

#include "../DatabaseHandler/DatabaseHandler.h"

#include <glog/logging.h>

using Poco::AutoPtr;
using Poco::Util::IniFileConfiguration;

int
BaseServer::main(const std::vector<std::string>& args)
{
  int loadedProperly = loadConfig();
  if (loadedProperly) {
    return loadedProperly;
  }
  std::shared_ptr<BaseServerRequestHandlerFactory> handlerFactory = nullptr;
  if (config().getBool("use_remote") && config().getBool("use_db"))
    handlerFactory = std::make_shared<BaseServerRequestHandlerFactory>(
      config().getInt("remote_port"),
      config().getString("remote_host"),
      _dbConfig);
  else if (config().getBool("use_db"))
    handlerFactory =
      std::make_shared<BaseServerRequestHandlerFactory>(0, "nohost", _dbConfig);
  else
    handlerFactory = std::make_shared<BaseServerRequestHandlerFactory>();

  Poco::Net::HTTPServer srv(handlerFactory.get(),
                            Poco::Net::ServerSocket(config().getInt("port")),
                            new Poco::Net::HTTPServerParams);

  srv.start();
  LOG(INFO) << "Server started on port " << config().getInt("port");

  waitForTerminationRequest();

  LOG(INFO) << "Shutting down...";
  srv.stop();
  return Application::EXIT_OK;
}

int
BaseServer::loadConfig()
{
  AutoPtr<IniFileConfiguration> pConf(new IniFileConfiguration("config.ini"));

  try {
    config().setInt("port", pConf->getInt("Config.port"));
    config().setBool("use_remote", pConf->getBool("Config.use_remote"));
    config().setBool("use_db", pConf->getBool("Config.use_db"));

    if (config().getBool("use_remote")) {
      config().setInt("remote_port", pConf->getInt("Config.remote_port"));
      config().setString("remote_host", pConf->getString("Config.remote_host"));
    }
    if (config().getBool("use_db")) {
      _dbConfig._database = pConf->getString("Config.db_dbname");
      _dbConfig._user = pConf->getString("Config.db_user");
      _dbConfig._password = pConf->getString("Config.db_password");
      _dbConfig._host = pConf->getString("Config.db_host");
      _dbConfig._port = pConf->getInt("Config.db_port");
    }
  } catch (Poco::NotFoundException& ex) {

    LOG(FATAL) << "[FAILED] " << ex.displayText();
    return Application::EXIT_CONFIG;
  }
  return Application::EXIT_OK;
}