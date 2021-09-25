#include "BaseServer.h"

#include <memory>

#include "Poco/Util/IniFileConfiguration.h"
#include "Poco/AutoPtr.h"

#include <glog/logging.h>

using Poco::AutoPtr;
using Poco::Util::IniFileConfiguration;

int BaseServer::main(const std::vector<std::string>& args)
{
    int loadedProperly = loadConfig();
    if (loadedProperly)
    {
        return loadedProperly;
    }

    auto handlerFactory = config().getBool("use_remote") ?
                          std::make_shared<BaseServerRequestHandlerFactory>(config().getInt("remote_port"), config().getString("remote_host")) :
                          std::make_shared<BaseServerRequestHandlerFactory>();

    Poco::Net::HTTPServer srv(handlerFactory.get(), Poco::Net::ServerSocket(config().getInt("port")), new Poco::Net::HTTPServerParams);

    srv.start();
    LOG(INFO) << "Server started on port " << config().getInt("port");

    waitForTerminationRequest();

    LOG(INFO) << "Shutting down...";
    srv.stop();
    return Application::EXIT_OK;
}

int BaseServer::loadConfig()
{
    AutoPtr<IniFileConfiguration> pConf(new IniFileConfiguration("config.ini"));

    try
    {
        config().setInt("port", pConf->getInt("Config.port"));
        config().setBool("use_remote", pConf->getBool("Config.use_remote"));

        if (config().getBool("use_remote"))
        {
            config().setInt("remote_port", pConf->getInt("Config.remote_port"));
            config().setString("remote_host", pConf->getString("Config.remote_host"));
        }
    }
    catch(Poco::NotFoundException& ex)
    {

        LOG(FATAL) << "[FAILED] " << ex.displayText();
        return Application::EXIT_CONFIG;
    }
    return Application::EXIT_OK;
}