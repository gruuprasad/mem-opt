# TAS-OPT

## Getting Started

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

## To test the Pass

Use `opt` tool to load the pass and invoke in llvm pass pipeline.
Running below command from `build` directory:

```
$ opt -load lib/libLLVMHelloFunction.so -hello < examples/hello_world.bc > /dev/null 
```
Expected output:

```
Hello: main
```
