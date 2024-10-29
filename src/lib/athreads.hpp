#include <iostream>

#define GAU8_INCREASE_THREAD_COUNT 1
#define GAU8_DECREASE_THREAD_COUNT 0


//TODO  Create some basic functions for thread throttling for GAU8. 
//DONE (bug) Create a func that calculates the ~auth requests / sec; 
//      Make that GAU8 always reduces/increases the number of threads to the most optimal based on the above function
namespace athreads{
    void start_requests_per_sec_thread(const long long* _attempts_counter);
    u_int16_t get_requests_per_second();
    void stop_requests_per_sec_thread();
}