#include <future>
#include <iostream>

class Callback {
public:
	virtual ~Callback() { std::cout << "Callback::~Callback()" << std:: endl; }
	virtual void run() { std::cout << "Callback::run()" << std::endl; }
};

class DB
{

  private:
	Callback *_callback;

  public:
    DB(): _callback(0) {std::cout << "DB::DB() Constructor"<< std:: endl;}
    ~DB() {std::cout << "DB::~DB() Destructor"<< std:: endl; delCallback();}
    void delCallback() { delete _callback; _callback = 0; }
    void setCallback(Callback *cb) {
        std::cout << "DB::setCallback() -> setting callback "<< std:: endl;
        delCallback(); _callback = cb;
    }

    void call() {
        std::cout << "DB::call() Trying to call host code ... "<< std:: endl;
        if (_callback) _callback->run();
    }
    void m1();

};