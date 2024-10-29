#include "connectors.hpp"
#include <cstdint>
#include <cstdio>
#include <libssh/libssh.h>


gau8::ssh::ssh(const char* _ip, const uint16_t& _port, int &_handle)
    : m_session(ssh_new())
{
    ssh_init();
    if(m_session == nullptr){

        _handle = 1;
    }else{
        _handle = 0;
    }

    ssh_options_set(m_session, SSH_OPTIONS_HOST , _ip);
    ssh_options_set(m_session, SSH_OPTIONS_HOST , &_port);
    if(ssh_connect(m_session) != SSH_OK)
    {
        _handle = 1;
    }else{
        _handle = 0;
    }

}
gau8::ssh::ssh(const char* _ip, const uint16_t& _port, const char* _username, int &_handle)
    : m_session(ssh_new()), m_username(_username)
{
    ssh_init();
    if(m_session == nullptr){
        _handle = 1;
    }else{
        _handle = 0;
    }

    ssh_options_set(m_session, SSH_OPTIONS_HOST , _ip);
    ssh_options_set(m_session, SSH_OPTIONS_HOST , &_port);
    if(ssh_connect(m_session) != SSH_OK)
    {
        _handle = 1;
    }else{
        _handle = 0;
    }
}


gau8::ssh::~ssh()
{
    ssh_disconnect(m_session);
    ssh_finalize();
    ssh_free(m_session);
}

bool gau8::ssh::auth_user_pass(const char* _username, const char* _password)
{

    int auth_result = ssh_userauth_password(m_session, _username, _password);

    if ( auth_result != SSH_AUTH_SUCCESS) {
        return false;
    }
    return true;
}


void gau8::ssh::send_cmd(const char* _cmd)
{
    ssh_channel f_channel = ssh_channel_new(m_session);
    uint16_t buffer_len = 0;
    char recv_buffer[256];

    if(f_channel == NULL)
    {
        std::cout << "Could not establish a channel with the target server" << std::endl;
        ssh_channel_free(f_channel);
        return;
    }
    if (ssh_channel_open_session(f_channel) != SSH_OK) {
        std::cout << "Error opening channel:" << ssh_get_error(m_session) << std::endl;
        ssh_channel_free(f_channel);
        // Handle error
    }

    if( ssh_channel_request_exec(f_channel, _cmd) != SSH_OK )
    {
        fprintf(stderr, "Could not exec \"%s\"\n %s", _cmd, ssh_get_error(m_session));
        ssh_channel_close(f_channel);
        ssh_channel_free(f_channel);
        return;
    }

    buffer_len = ssh_channel_read(f_channel, recv_buffer, sizeof(recv_buffer), 0);

    while (buffer_len) {
        if (write(1, recv_buffer, buffer_len) != buffer_len)
        {
            ssh_channel_close(f_channel);
            ssh_channel_free(f_channel);
        }
        buffer_len = ssh_channel_read(f_channel, recv_buffer, sizeof(recv_buffer), 0);
    }

    fprintf(stdout, "Command \"%s\" executed successfully!\n", _cmd);
    ssh_channel_close(f_channel);
    ssh_channel_free(f_channel);
}



