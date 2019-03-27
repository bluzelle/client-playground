#include <iostream>

class DB
{

private:
public:
  DB() {}
  ~DB() {}

  void listen_many();
  int slowGet(int milliseconds);
};