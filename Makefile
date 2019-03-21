SRC = src/cpp
LIB = lib
CC = g++
FLAGS = -fPIC -pthread -std=c++14

# the python interface through swig
PYTHONI = `python3-config --cflags`
PYTHONL = `python3-config --ldflags`

all:
	$(shell mkdir -p $(LIB))
	$(shell cp $(SRC)/libdb.h $(LIB))
	$(shell mkdir -p $(LIB))
	$(CC) $(FLAGS) -c $(SRC)/libdb.cpp -o $(LIB)/libdb.o
	swig -c++ -python -o $(LIB)/libdb_wrap.cxx $(SRC)/libdb.i
	$(CC) $(FLAGS) $(PYTHONI) -c $(LIB)/libdb_wrap.cxx -o $(LIB)/libdb_wrap.o
	$(CC) $(PYTHONL) $(LIBFLAGS) -shared $(LIB)/libdb.o $(LIB)/libdb_wrap.o -o $(LIB)/_libdb.so

clean:
	-rm -rf $(LIB)
