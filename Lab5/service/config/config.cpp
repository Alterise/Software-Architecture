#include "config.h"

Config::Config() {
    _db_host = std::getenv("DB_HOST");
    _db_port = std::getenv("DB_PORT");
    _db_login = std::getenv("DB_LOGIN");
    _db_password = std::getenv("DB_PASSWORD");
    _db_database = std::getenv("DB_DATABASE");
    _cache_addr = std::getenv("CACHE_ADDR");
}

Config &Config::get() {
    static Config _instance;
    return _instance;
}

const std::string &Config::get_db_port() const {
    return _db_port;
}

const std::string &Config::get_db_host() const {
    return _db_host;
}

const std::string &Config::get_db_login() const {
    return _db_login;
}

const std::string &Config::get_db_password() const {
    return _db_password;
}
const std::string &Config::get_db_database() const {
    return _db_database;
}

const std::string &Config::get_cache_addr() const {
    return _cache_addr;
}