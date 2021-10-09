#include "BaseServer/BaseServer.h"
#include <glog/logging.h>

int
main(int argc, char** argv)
{
  FLAGS_log_dir = "/application/logs";
  // FLAGS_log_dir = "/tmp"; // those two are only for debugginh purposes
  // FLAGS_logtostderr = 1;
  google::InitGoogleLogging(argv[0]);
  LOG(INFO) << "Logger initialized";
  BaseServer app;
  return app.run(argc, argv);
}