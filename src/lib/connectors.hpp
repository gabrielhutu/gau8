#pragma once
#include <cstdint>
#include <fstream>
#include <libssh/libssh.h>
#include <iostream>
#include <curl/curl.h>

#ifndef GAU8_LICENSE
    #define GAU8_LICENSE
    #define API_DOMAIN "https://example.com/"
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
    license(const char* user_id);
    ~license();
    bool check_license();
};

}