# TAS-OPT

## Getting Started
TAS-OPT implements optimisation to enhance cache locality in network packet processing code.
It also contains contorl flow linearization transform.

### Prerequisites

* `g++` or `clang++`
* `cmake `3.4.3 or later
* `LLVM framework` 8.0.1
* `git`

### Building

```
$ git clone git-rts@gitlab.mpi-sws.org:gruu/tas-opt.git
$ cd tas-opt
$ mkdir build && cd build
$ cmake ..
$ make
```

## To run unit tests

```
$ cd build/bin
command: ./tests <TEST_DIR_PATH>
In our case test files are in  $PROJECT_ROOT/test/unittests/input
$./tests ../../test/unittests/input/
```

## Running tool
```
$ cd build/bin
$ ./tasopt -c <input C source file>
## Documentation
Few design notes can be found in the project wiki.
