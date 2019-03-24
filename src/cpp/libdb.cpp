#include "libdb.h"
#include <chrono>
#include <thread>

void DB::slowGet(int milliseconds)
{
    std::cout << "DB::slowGet() sleep START, obj_id =  "+ std::to_string(reinterpret_cast<long>(this)) <<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    std::cout << "DB::slowGet() sleep STOP, obj_id =  "+ std::to_string(reinterpret_cast<long>(this)) <<std::endl;
}