# Engine [![TravisCI](https://travis-ci.org/cpcdoy/Engine.svg?branch=dev)](https://travis-ci.org/cpcdoy/Engine)
Rendering Engine

## Table of content
1. [Build](#Build)

2. [Architecture](#Archi)

3. [Example usage](#Example)

4. [Appendix](#Appendix)

A real-time 3D rendering engine.


<a name="Build"/>
## Build
### Dependencies

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
