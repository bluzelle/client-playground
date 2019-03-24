#include <future>
#include <iostream>
#include <stdlib.h>
#include <time.h>

class Callback {
public:
	virtual ~Callback() { }
	virtual void run() { std::cout << "Callback::run()" << std::endl; }
};

class DB
{

  private:
	Callback *_callback;

  public:
    DB(): _callback(0) {
    }
    ~DB() {delCallback();}
    void delCallback() { delete _callback; _callback = 0; }
    void setCallback(Callback *cb) {
        delCallback(); _callback = cb;
    }

    void call() {
        if (_callback) _callback->run();
    }
    void slowGet(int milliseconds);

};