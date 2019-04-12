#include "libdb.h"
#include <chrono>
#include <thread>
#include <cstdlib>
#include <future>
#include <iostream>
#include <time.h>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

using namespace boost::asio;

using boost::asio::ip::tcp;
using ip::tcp;
using std::cout;
using std::endl;
using std::string;

class session
{
  public:
    session(boost::asio::io_service &io_service)
        : socket_(io_service)
    {
    }

    tcp::socket &socket()
    {
        return socket_;
    }

    void start()
    {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&session::handle_read, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }

    void handle_read(const boost::system::error_code &error,
                     size_t bytes_transferred)
    {
        if (!error)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(data_, bytes_transferred),
                                     boost::bind(&session::handle_write, this,
                                                 boost::asio::placeholders::error));
        }
        else
        {
            delete this;
        }
    }

    void handle_write(const boost::system::error_code &error)
    {
        if (!error)
        {
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                    boost::bind(&session::handle_read, this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        }
        else
        {
            delete this;
        }
    }

  private:
    tcp::socket socket_;
    enum
    {
        max_length = 1024
    };
    char data_[max_length];
};

class server
{
  public:
    server(boost::asio::io_service &io_service, short port)
        : io_service_(io_service),
          acceptor(io_service, tcp::endpoint(tcp::v4(), port))
    {
        session *new_session = new session(io_service_);
        acceptor.async_accept(new_session->socket(),
                              boost::bind(&server::handle_accept, this, new_session,
                                          boost::asio::placeholders::error));
    }

    void handle_accept(session *new_session,
                       const boost::system::error_code &error)
    {
        if (!error)
        {
            new_session->start();
            new_session = new session(io_service_);
            acceptor.async_accept(new_session->socket(),
                                  boost::bind(&server::handle_accept, this, new_session,
                                              boost::asio::placeholders::error));
        }
        else
        {
            delete new_session;
        }
    }

  private:
    boost::asio::io_service &io_service_;
    tcp::acceptor acceptor;
};

void DB::listen_many()
{
    auto f = [](int params) {
        try
        {
            boost::asio::io_service io_service;

            using namespace std;
            server server(io_service, 1234);

            io_service.run();
        }
        catch (std::exception &e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
        }

        return 0;
    };
    std::thread thread_object(f, 1);
    thread_object.detach();
}


class udp_response : public response
{
public:
    udp_response()
    {
        std::cout << "response::udp_response() NEW @ " + std::to_string(reinterpret_cast<long>(this)) << std::endl;
//        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//        if (!sock)
//        {
//            throw std::runtime_error("unable to create udp socket");
//        }
//
//        sockaddr_in local;
//        local.sin_family = AF_INET;
//        local.sin_addr.s_addr = INADDR_LOOPBACK;
//        local.sin_port = 20001; //randomly selected port
//        if (bind(sock, (sockaddr*)&local, sizeof(local)) == -1)
//        {
//            throw std::runtime_error("unable to bind udp socket");
//        }
//
//        struct sockaddr_in sin;
//        socklen_t addrlen = sizeof(sin);
//        if (getsockname(sock, (struct sockaddr *)&sin, &addrlen) == 0
//            && sin.sin_family == AF_INET && addrlen == sizeof(sin))
//        {
//            my_id = ntohs(sin.sin_port);
//        }
//        else
//        {
//            throw std::runtime_error("error determining local port");
//        }
    }
    ~udp_response(){
        std::cout << "response::~udp_response() DESTRUCT @ " + std::to_string(reinterpret_cast<long>(this)) << std::endl;
    }

    udp_response(const response&){
        std::cout << "response::~response(const response&)() COPY @ " + std::to_string(reinterpret_cast<long>(this)) << std::endl;
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


int DB::slowGet(int milliseconds)
{
    std::cout << "DB::slowGet() sleep START, obj_id =  " + std::to_string(reinterpret_cast<long>(this)) << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    std::cout << "DB::slowGet() sleep STOP, obj_id =  " + std::to_string(reinterpret_cast<long>(this)) << std::endl;
    return 5;
}


DB::~DB(){
}

DB::DB():
        responseSharedPtr(new udp_response){
};

std::shared_ptr<response> DB::getResponseSharedPtr(){
    return responseSharedPtr;
}

int DB::getResponseSharedPtrCount(){
    return responseSharedPtr.use_count();
}

//response DB::createAndReturnResponse(){
//    return new udp_response;
//}


response *DB::createAndReturnResponseRawPtr(){
    return new udp_response;
}

void DB::deleteResponseRawPtr(response * resp ){
    delete resp;
}