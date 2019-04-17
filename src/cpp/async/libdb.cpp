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
#include <unistd.h>

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


class testimpl: public test{

private:
    boost::asio::io_context io_context;
    std::thread *io_thread;
    boost::asio::signal_set *signals = nullptr;
    std::shared_ptr<response> responseSharedPtr;

public:
    testimpl(){

        std::cout << "test::test called" << std::endl;

        // setup signal handler...
        signals = new boost::asio::signal_set(io_context, SIGINT);
        signals->async_wait([&](const boost::system::error_code& error, int signal_number)
        {
            if (!error)
            {
                std::cout << "signal received -- shutting down (" << signal_number << ")";
                io_context.stop();
            }
            else
            {
                std::cout << "Error: " << error.value() << ", " << error.category().name() << std::endl;
            }
        });

        this->io_thread = new std::thread([&]()
        {
            std::cout << "running io_context" << std::endl;
            auto res = io_context.run();
            std::cout << "asio loop finished! count = " << res << std::endl;
        });

    }

    ~testimpl(){
        std::cout << "test::~test called" << std::endl;
        this->io_context.stop();
        this->io_thread->join();
        delete this->io_thread;
    }

    std::shared_ptr<response> makeResponseSharedPtr(){
       return std::make_shared<udp_response>();
    }


    int getResponseSharedPtrCount(){
        return responseSharedPtr.use_count();
    }


    void setResponseSharedPtr(std::shared_ptr<response> ptr){
        responseSharedPtr = ptr;
    }


    void run_timer(int t){
        std::cout << "test::run_timer called with value " << t << std::endl;
        auto timer = std::make_shared<boost::asio::steady_timer>(io_context, boost::asio::chrono::seconds(t));
        timer->async_wait([timer, this](const boost::system::error_code& error)
        {
            if (!error)
            {
                std::cout << "timer fired" << std::endl;
                responseSharedPtr.get()->set_ready();
                responseSharedPtr.get()->set_result("CPP_RESULT");
            }
            else
            {
                std::cout << "Error: " << error.value() << ", " << error.category().name() << std::endl;
            }
        });
    }
    void do_something(){
        std::cout << "test::do_something called" << std::endl;
    }

    int increment_an_int(int x){
        std::cout << "test::increment_an_int called" << std::endl;
        return x + 1;
    }
};

std::shared_ptr<test> DB::newTest(){
    return std::make_shared<testimpl>();
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

