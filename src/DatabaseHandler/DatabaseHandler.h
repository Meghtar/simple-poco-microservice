#pragma once
#include "Poco/MongoDB/Array.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Cursor.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/MongoDB.h"

#include <memory>

struct DatabaseConfig
{
  std::string _host;
  int _port;
  std::string _database;
  std::string _user;
  std::string _password;
};

class DatabaseHandler
{
public:
  DatabaseHandler(const DatabaseConfig& config)
    : _config(config)
  {
    connect();
  }
  std::vector<Poco::SharedPtr<Poco::MongoDB::Document>>
  getAllDocumentsFromCollection(const std::string& collectionName)
  {
    Poco::SharedPtr<Poco::MongoDB::QueryRequest> queryPtr =
      _db->createQueryRequest(collectionName);
    Poco::MongoDB::ResponseMessage response;
    _connection->sendRequest(*queryPtr, response);
    return response.documents();
  }

private:
  void connect()
  {
    if (!_connection) {
      LOG(INFO) << "Connecting to database on host " << _config._host << ":"
                << _config._port;
      _connection = std::make_shared<Poco::MongoDB::Connection>(_config._host,
                                                                _config._port);
      _db = std::make_shared<Poco::MongoDB::Database>(_config._database);
      _db->authenticate(*_connection, _config._user, _config._password);
    }
  }

  DatabaseConfig _config;
  std::shared_ptr<Poco::MongoDB::Connection> _connection;
  std::shared_ptr<Poco::MongoDB::Database> _db;
};
