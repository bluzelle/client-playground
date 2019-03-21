#include <stdio.h>
#include "lib.h"

lib::lib()
{
    printf("Constructor\n");
    variable = 100;
}

lib::~lib()
{
    printf("Destructor\n");
    variable = 0;
}

void lib::m1()
{
    printf("ping!\n");
}

// int lib::called_from_async()
// {
//     printf("Async call");
//     return 1;
// }

// std::future<int> lib::start_async(){
//     auto result(std::async(called_from_async));
//     return result;
// }