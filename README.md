### Building the example

#### OSX

```text
$ mkdir build
$ cd build
$ cmake .. -DPYTHON=ON -DGOLANG=OFF
$ cmake --build .
```

#### Ubuntu

```text
$ mkdir build
$ cd build
$ ~/mycmake/bin/cmake .. -DPYTHON=ON -DGOLANG=OFF
$ sudo cmake --build .
```

### Running

Golang version: start `golang_run.sh`
Python version: start `py_run.sh`
