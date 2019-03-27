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

using ip::tcp;
using std::string;
using std::cout;
using std::endl;
using boost::asio::ip::tcp;

int DB::slowGet(int milliseconds)
  {
    std::cout << "DB::slowGet() sleep START, obj_id =  " + std::to_string(reinterpret_cast<long>(this)) << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    std::cout << "DB::slowGet() sleep STOP, obj_id =  " + std::to_string(reinterpret_cast<long>(this)) << std::endl;
    return 5;
  }


class session
{
public:
    session(boost::asio::io_service& io_service)
            : socket_(io_service)
    {
    }

    tcp::socket& socket()
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

    void handle_read(const boost::system::error_code& error,
                     size_t bytes_transferred)
    {
        if (!error)
        {
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

    void handle_write(const boost::system::error_code& error)
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
    enum { max_length = 1024 };
    char data_[max_length];
};

class server
{
public:
    server(boost::asio::io_service& io_service, short port)
            : io_service_(io_service),
              acceptor(io_service, tcp::endpoint(tcp::v4(), port))
    {
        session* new_session = new session(io_service_);
        acceptor.async_accept(new_session->socket(),
                               boost::bind(&server::handle_accept, this, new_session,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(session* new_session,
                       const boost::system::error_code& error)
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
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor;
};

void DB::listen_many(){
       auto f = [](int params) {
        try
        {
            boost::asio::io_service io_service;

            using namespace std;
            server server(io_service, 1234);

            io_service.run();
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
        }

        return 0;
    };
    std::thread thread_object(f, 1);
    thread_object.detach();
    }
