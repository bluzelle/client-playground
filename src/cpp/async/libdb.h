#include <iostream>


class response
    {
    public:
        virtual ~response() = default;

        // consumer
        int get_signal_id(int theirs)
        {
            this->their_id = theirs;
            return this->my_id;
        }

        bool is_ready()
        {
            return this->ready;
        }

        std::string get_result()
        {
            if (ready)
            {
                return this->result_str;
            }

            return {};
        }

//        std::shared_ptr<database> get_db()
//        {
//            return this->db;
//        }

        // producer
        void set_result(const std::string& result)
        {
            this->result_str = result;
        }

        void set_ready()
        {
            this->set_error(0);
        }

        void set_error(int error)
        {
            this->ready = true;
            this->signal(error);
        }

//        void set_db(std::shared_ptr<database> db_ptr)
//        {
//            this->db = db_ptr;
//        }

        virtual void signal(int error) = 0;

    protected:
        int my_id = 0;
        int their_id = 0;
        std::string result_str;
        std::atomic<bool> ready = ATOMIC_VAR_INIT(false);
        //std::shared_ptr<database> db;


};


class test{
public:
        test(){};
        ~test() {};
        virtual void process_request(int timeout) = 0;
        virtual void do_something() = 0;
        virtual int increment_an_int(int x) = 0;
        virtual std::shared_ptr<response> makeResponseSharedPtr() = 0;
        virtual void setResponseSharedPtr(std::shared_ptr<response>) = 0;
        virtual int getResponseSharedPtrCount() = 0;
};



class DB
{

public:
  DB();
  ~DB();

  void listen_many();
  int slowGet(int milliseconds);
  std::shared_ptr<test> newTest();
};

