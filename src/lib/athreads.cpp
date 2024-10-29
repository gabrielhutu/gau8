#include "./athreads.hpp"
#include <thread>
#include <iostream>
#include <mutex>


std::mutex m_lock;
long long counter = 0, previous_counter = 0; 
std::thread calc_req_per_sec;

u_int16_t athreads::get_requests_per_second(){
    return counter - previous_counter;
}

void athreads::start_requests_per_sec_thread(const long long* _attempts_counter)
{

    calc_req_per_sec = std::thread([&](){
        if(m_lock.try_lock())
        {
            previous_counter = counter;            
            counter = *_attempts_counter;
        }
        m_lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    });
}

void athreads::stop_requests_per_sec_thread()
{
    calc_req_per_sec.detach();
}