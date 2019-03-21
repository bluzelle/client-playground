SRC = src/cpp
LIB = lib
CC = g++
FLAGS = -fPIC -pthread -std=c++14

# the python interface through swig
PYTHONI = `python3-config --cflags`
PYTHONL = `python3-config --ldflags`

all:
	$(shell mkdir -p $(LIB))
	$(shell cp $(SRC)/lib.h $(LIB))
	$(shell mkdir -p $(LIB))
	$(CC) $(FLAGS) -c $(SRC)/lib.cpp -o $(LIB)/lib.o
	swig -c++ -python -o $(LIB)/lib_wrap.cxx $(SRC)/lib.i
	$(CC) $(FLAGS) $(PYTHONI) -c $(LIB)/lib_wrap.cxx -o $(LIB)/lib_wrap.o
	$(CC) $(PYTHONL) $(LIBFLAGS) -shared $(LIB)/lib.o $(LIB)/lib_wrap.o -o $(LIB)/_lib.so

clean:
	-rm -rf $(LIB)
