#include <stdio.h>
#include <future>

class lib
{
  public:
    lib();
    ~lib();

    void m1();
    void m2() { printf("from m2 \n"); }
    // static int called_from_async();
    // std::future<int> start_async();

  private:
    int variable;
};