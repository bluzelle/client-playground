#include <iostream>

class response
{
public:
    virtual ~response() = default;

    // consumer
    int get_signal_id(int theirs)
    {
        this->their_id = theirs;
        return my_id;
    }

    bool is_ready()
    {
        return ready;
    }

    std::string get_reponse()
    {
        if (ready)
        {
            return result_str;
        }

        return {};
    }

    // producer
    void set_result(const std::string& result)
    {
        result_str = result;
    }

    void set_ready()
    {
        ready = true;
        signal(0);
    }

    void set_error(int error)
    {
        ready = true;
        signal(error);
    }

protected:
    int my_id = 0;
    int their_id = 0;
    std::string result_str;
    bool ready = false;

    virtual void signal(int error) = 0;
};

class DB
{

private:
public:
  DB();
  ~DB();

  void listen_many();
  int slowGet(int milliseconds);
};

