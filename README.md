# Engine

[![Travis CI](https://img.shields.io/travis/rust-lang/rust/master.svg?style=flat-square)](https://travis-ci.org/cpcdoy/Engine.svg?branch=dev)
[![Trello](https://img.shields.io/badge/trello-todo%20list-blue.svg?style=flat-square)](https://trello.com/b/B5hQOW6n/engine)



A 3D real-time rendering Engine

## Table of content
1. [Build](#Build)

2. [Architecture](#Archi)

3. [Example usage](#Example)

4. [Task list](#TODO)

5. [Appendix](#Appendix)

<a name="Build"/>

##  Build

###  Dependencies

Ubuntu:

    apt-get update && apt-get install -y \
    build-essential \
    libglm-dev \
    libglfw3-dev \
    libglew-dev \
    libsoil-dev \
    bison \
    flex \
    git \
    cmake \
    pkg-config

### In-source build

`cmake .`

`make` (to build the shared object `.so` of the engine that can be linked with your application)

or

`make check` (used to build the engine's test suite)

`./Engine_test`

`make deep_clean` (A special `make` target to clean eveything that `cmake` generates)

### Out-of-source build

`mkdir build`

`cd build`

`cmake ..`

`make` or `make check`

<a name="TODO"/>

## Task list

Check the TO DO list [here](https://trello.com/b/B5hQOW6n/engine)

<a name="Archi"/>

## Architecture
Here is the current architecture:

`TODO`


<a name="Example"/>

## Example usage

[Here](https://github.com/cpcdoy/Engine/blob/dev/tests/debug/main.cc), you can take a look at an example.

<a name="Appendix"/>

## Appendix

### Articles

- [SSAO](https://hackercorp.eu/ssao.html)
