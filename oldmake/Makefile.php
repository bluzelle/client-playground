BOOSTI = -I/usr/local/Cellar/boost/1.69.0/include
FLAGS = -fPIC -pthread -std=c++14
CC = g++

all:
	# swig -c++ -php7 libdb.i
	# $(CC) `php-config --includes` -Wl,-undefined,dynamic_lookup $(BOOSTI) $(FLAGS) libdb_wrap.cxx libdb.cpp
	# $(CC) -shared libdb_wrap.o libdb.o -o libdb.so
	#-Wl,-undefined,dynamic_lookup
	#gcc -undefined dynamic_lookup -shared libdb_wrap.o libdb.o -o libdb.so
	swig -c++ -php7 libdb.i
	g++ `php-config --includes` -Wl,-undefined,dynamic_lookup -I/usr/local/Cellar/boost/1.69.0/include -fPIC -pthread -std=c++14 -c libdb_wrap.cxx libdb.cpp
	g++ -Wl,-undefined,dynamic_lookup -shared libdb_wrap.o libdb.o -o libdb.so
	#-Wl,-undefined,error.
