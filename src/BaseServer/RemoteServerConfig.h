#pragma once

#include <string>

struct RemoteServerConfig
{
  RemoteServerConfig(const std::string& host, const uint16_t port)
    : _remoteServicePort(port)
    , _remoteServiceHost(host)
    , _useRemote(true)
  {}
  RemoteServerConfig()
    : _remoteServicePort(0)
    , _remoteServiceHost("")
    , _useRemote(false)
  {}
  uint16_t getPort() const { return _remoteServicePort; }
  std::string getHost() const { return _remoteServiceHost; }
  bool useRemote() const { return _useRemote; }

private:
  const uint16_t _remoteServicePort;
  const std::string _remoteServiceHost;
  const bool _useRemote;
};