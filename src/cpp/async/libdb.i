%module libdb

%include <std_shared_ptr.i>
%{
#include "libdb.h"
#include "boost/asio.hpp"
%}

%shared_ptr(response)
%include "libdb.h"
