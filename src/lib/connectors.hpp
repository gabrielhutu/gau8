#pragma once
#include <cstdint>
#include <fstream>
#include <libssh/libssh.h>
#include <iostream>

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

};}