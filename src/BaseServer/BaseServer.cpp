#include "BaseServer.h"

#include <memory>
#include <optional>

#include "Poco/Util/IniFileConfiguration.h"
#include "Poco/AutoPtr.h"

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
    std::cout << "Server started on port " << config().getInt("port") << std::endl;

    waitForTerminationRequest();

    std::cout << "Shutting down..." << std::endl;
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
    catch(NotFoundException& ex)
    {
        std::cout << "[FAILED] " << ex.displayText() << std::endl;
        return Application::EXIT_CONFIG;
    }
    return Application::EXIT_OK;
}