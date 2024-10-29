#include "./useful.hpp"
#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <sys/ioctl.h>
#include <mutex>

std::mutex m_lock;

winsize w;
void print_term_size_cr(const std::string& _str)
{
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    if(m_lock.try_lock())
    {
        std::cout << std::left << std::setw(w.ws_col - 1) << std::setfill(' ') << _str;
        std::cout << "\r";
        m_lock.unlock();
    }
}