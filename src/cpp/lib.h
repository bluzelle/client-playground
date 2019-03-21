#include <future>
#include <iostream>

class Callback {
public:
	virtual ~Callback() { std::cout << "Callback::~Callback()" << std:: endl; }
	virtual void run() { std::cout << "Callback::run()" << std::endl; }
};

class lib
{

  private:
	Callback *_callback;

  public:
    lib(): _callback(0) {std::cout << "lib::lib() Constructor"<< std:: endl;}
    ~lib() {std::cout << "lib::~lib() Destructor"<< std:: endl; delCallback();}
    void delCallback() { delete _callback; _callback = 0; }
    void setCallback(Callback *cb) {
        std::cout << "lib::setCallback() -> setting callback "<< std:: endl;
        delCallback(); _callback = cb;
    }

    void call() {
        std::cout << "lib::call() Trying to call host code ... "<< std:: endl;
        if (_callback) _callback->run();
    }
    void m1();

};