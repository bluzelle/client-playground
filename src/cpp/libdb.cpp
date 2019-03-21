#include "libdb.h"
#include <chrono>
#include <thread>

void DB::slowGet(int milliseconds)
{
    std::cout << "DB::slowGet() sleep START ... " <<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    std::cout << "DB::slowGet() sleep FINISH ... " <<std::endl;
}