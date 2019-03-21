%module(directors="1") lib

%{
#include "lib.h"

%}

/* turn on director wrapping Callback */
%feature("director") Callback;

%include "lib.h"