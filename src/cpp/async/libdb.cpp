#include "libdb.h"
#include <chrono>
#include <thread>
#include <cstdlib>
#include <future>
#include <iostream>
#include <time.h>
#include <string>
#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string.h>

#include <boost/bind.hpp>
#include <boost/asio.hpp>


using namespace boost::asio;

using boost::asio::ip::tcp;
using ip::tcp;
using std::cout;
using std::endl;
using std::string;

class udp_response : public response
    {
    public:
        udp_response()
        {
            sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (!sock)
            {
                throw std::runtime_error("unable to create udp socket");
            }

            sockaddr_in local;
            memset(&local, 0, sizeof(sockaddr_in));
            local.sin_family = AF_INET;
            local.sin_addr.s_addr = INADDR_ANY;
            local.sin_port = 1235; //randomly selected port
            if (bind(sock, (sockaddr*)&local, sizeof(local)) == -1)
            {
                throw std::runtime_error("unable to bind udp socket");
            }

            struct sockaddr_in sin;
            socklen_t addrlen = sizeof(sin);
            if (getsockname(sock, (struct sockaddr *)&sin, &addrlen) == 0
                && sin.sin_family == AF_INET && addrlen == sizeof(sin))
            {
                my_id = ntohs(sin.sin_port);
            }
            else
            {
                throw std::runtime_error("error determining local port");
            }
        }

        void signal(int error) override
        {
            struct sockaddr_in their_addr;
            their_addr.sin_family = AF_INET;
            their_addr.sin_port = htons(their_id);
            their_addr.sin_addr.s_addr = INADDR_LOOPBACK;
            sendto(sock, &error, sizeof(error), 0, (sockaddr*)&their_addr, sizeof(their_addr));
        }

        int sock;
};


std::shared_ptr<response> DB::makeResponseSharedPtr(){
   return std::make_shared<udp_response>();
}

void DB::listen_many()
{

}

int DB::slowGet(int milliseconds)
{
    std::cout << "DB::slowGet() sleep START, obj_id =  " + std::to_string(reinterpret_cast<long>(this)) << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    std::cout << "DB::slowGet() sleep STOP, obj_id =  " + std::to_string(reinterpret_cast<long>(this)) << std::endl;
    return 5;
}


DB::~DB(){
}

DB::DB(){
};


int DB::getResponseSharedPtrCount(){
    return responseSharedPtr.use_count();
}


void DB::setResponseSharedPtr(std::shared_ptr<response> ptr){
    responseSharedPtr = ptr;
}