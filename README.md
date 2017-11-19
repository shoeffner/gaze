# gaze

[![Build Status](https://semaphoreci.com/api/v1/projects/1c4f3523-8438-462d-a344-993e66c7f978/1595933/shields_badge.svg)](https://semaphoreci.com/hoeffner/gaze)

## Dependencies

You need the following dependencies to build gaze (tested versions in
parentheses):
- C++17
- CMake (3.10.0-rc2)
- OpenCV (3.3.0)
- dlib (19.7)
- yaml-cpp ([5a519a4](https://github.com/jbeder/yaml-cpp/commit/5a519a4c6cb03a2aa440b8d53a829ef2db30bb9e), will be cloned)


### Demos

For the examples/demo projects you need:

**Simple Tracking**:
- OpenCV (3.3.0)

**Where People Look**:
- Boost (1.65.1)
- pkg-config (0.29.2)
- gtk+3 (3.22.24)
- glib (2.54.1)

### Documentation

- Doxygen (1.8.13)
- Python 3 (3.6.3)
- git (2.14.2)

### Additional gimmicks and gadgets

**ctags**:
You can build ctags files (e.g. to navigate around the source in vim).
- ctags (5.8)


## Building

### Library

Create a `build` directory and call `cmake`, followed by `make` to build the
gaze library:

```bash
mkdir
build
cd build
cmake ..
make
make install
```


### Demos

To build the demo programs, you need to call cmake with `-DBUILD_EXAMPLES=ON`:

```bash
mkdir build
cd build
cmake -DBUILD_EXAMPLES=ON ..
make
make install
```


#### `where_people_look`

This is a reimplementation of Judd et al. (2009)'s data acquisition experiment \cite judd2009.

It needs to be *installed* using `make install` to put the images into the
correct location. You can run it by simply typing `./where_people_look` from
the build directory.


#### `simple_tracking`

This program just starts a small window with an "experiment" and tracks gaze.
Starts the gaze tracker in debug mode to see what is tracked.


### Documentation

You can always build the documentation using `make docs`.
Or you can find it [here](https://shoeffner.github.io/gaze).
