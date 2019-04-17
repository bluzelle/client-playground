%module libdb

%include <std_shared_ptr.i>
%{
#include "libdb.h"
#include "boost/asio.hpp"
%}

%shared_ptr(response)
%shared_ptr(test)
%typemap(out) std::string {
        $result = PyString_FromString($1.c_str());
}
%include "libdb.h"
