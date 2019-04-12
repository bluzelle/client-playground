%module libdb

namespace std {
        template < class T >
        class shared_ptr
        {
            public:
                shared_ptr( const shared_ptr<T>& );
                T* operator->();
                ~shared_ptr();
        };
};


%{
#include "libdb.h"
#include "boost/asio.hpp"
%}
%include "libdb.h"