#include "BaseServer/BaseServer.h"
#include <glog/logging.h>

int main(int argc, char** argv)
{
    FLAGS_log_dir = "/application/logs";
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "Logger initialized";
    BaseServer app;
    return app.run(argc, argv);
}