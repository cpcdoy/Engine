# Engine
Rendering Engine

A rendering engine that works based on plug-ins that can be easily added to the code.

## Architecture
Here is the current architecture:

`TODO`

## Build
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

## Appendix

Here's an [article](https://hackercorp.eu/ssao.html) about one method used in this engine
