#pragma once
#include <cstdint>
#include <fstream>
#include <libssh/libssh.h>
#include <iostream>
#include <curl/curl.h>

#ifndef GAU8_LICENSE
    #define GAU8_LICENSE
    //Number of chars to ignore before reading the actual hash from the GET request
    #define START_KEY_INDEX 23
    //SECRET is the value used for the hash stored in the DB
    #define SECRET 123091203
    //User ID
    #define USER_ID "664a6d7f87b5922ca5e4fb7d"
    #define API_DOMAIN "http://64.226.98.128:8001/"
#endif

namespace gau8 {
class ssh{ 

private:
    ssh_session m_session;
    const char* m_username;
public:
    ssh(const char* _ip, const uint16_t& _port, int& _handle);
    ssh(const char* _ip, const uint16_t& _port, const char* _username, int &_handle);
    ~ssh();
    bool auth_user_pass(const char* _username, const char* _password);
    void send_cmd(const char* _cmd);

};

class license{
private:

    CURL* handle;
    const std::string api;

public:
    license();
    ~license();
    bool check_license();
};

}