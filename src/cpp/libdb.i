%module(directors="1") libdb

%{
#include "libdb.h"

%}

/* turn on director wrapping Callback */
%feature("director") Callback;

%include "libdb.h"