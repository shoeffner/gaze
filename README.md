# Gaze

[![Build Status](https://semaphoreci.com/api/v1/projects/1c4f3523-8438-462d-a344-993e66c7f978/1574305/badge.svg)](https://semaphoreci.com/hoeffner/gaze)

[Documentation](https://shoeffner.github.io/gaze) &ndash; [Thesis](https://shoeffner.github.io/mthesis)

Gaze aims at providing an easy to use gaze tracking library. It was the project for my master's thesis.
It is inspired by [eyeLike](https://github.com/trishume/eyeLike), and performs well in the task of eye tracking.

*Warning:* Gaze tracking does not work!

But you can try the `GazeCapture` pipeline step by modifying the `gaze.yaml` and configuring Gaze with `--caffe`. However, since the underlying CNN is trained on iPhone and iPad screens, it is most accurate in a smaller screen area around the camera.

If you are interested in picking up the project or have some ideas on how to best fix the gaze tracking, make sure to drop me a line! [shoeffner@uos.de](mailto:shoeffner@uos.de)


## Dependencies

You need the following dependencies to build gaze (tested versions in
parentheses):

- C++17 (clang-900.0.39.2)
- CMake (3.10.0-rc2, 3.10.2)
- OpenCV (3.3.0, 3.4.0)
- dlib (at least 19.8 (requires davisking/dlib#957))
- yaml-cpp ([5a519a4](https://github.com/jbeder/yaml-cpp/commit/5a519a4c6cb03a2aa440b8d53a829ef2db30bb9e), will be cloned)

For the examples/demo projects you need:

- OpenCV (3.3.0)
- Boost (1.65.1)
- pkg-config (0.29.2)
- gtk+3 (3.22.24)
- glib (2.54.1)

For the documentation you need:

- Doxygen (1.8.13)
- Python 3 (3.6.3)
- git (2.14.2)

For the development you need:

- cpplint

Optional:

- ctags (5.8)


## Building

To build gaze, simply run

```bash
./configure.sh
cd build
make install
```

`configure.sh` comes with some options:

- `--release`: Enables compiler optimizations
- `--debug`: Disables compiler optimizations, builds debug symbols and creates verbose makefiles.
- `--examples`: Configures cmake to create build files for the examples.
- `--doc`: Configures cmake to create build files for the documentation.
- `--tagfiles`: Links the documentation to other projects (OpenCV and the C++ standard library)
- `--dry`: Just prints the `cmake ..` command without executing it
- `--caffe`: Enables a build with Caffe support. This allows to use the experimental pipeline step `GazeCapture`.


## Demo programs

### `where_people_look`

This is a reimplementation of Judd et al. (2009)'s data acquisition experiment @cite Judd2009.

It needs to be *installed* using `make install` to put the images into the
correct location. You can run it by simply typing `./where_people_look` from
the build directory.


### `simple_tracking`

This program just starts a small window with an "experiment" and tracks gaze.
Starts the gaze tracker in debug mode to see what is tracked.


## Development

Install [cpplint](https://github.com/cpplint/cpplint).

Add a pre-commit hook (`.git/hooks/pre-commit`):
```
cpplint --recursive --extensions cpp,h --quiet src tests include
```


### License

Gaze is released under the [MIT License](LICENSE.md).

But it cannot be used for commercial use without a modification,
quoting from [dlib-model's README](https://github.com/davisking/dlib-models/blob/ae50fe33583de33c60276611d37915e93d11566b/README.md):

> The license for this dataset excludes commercial use and Stefanos Zafeiriou,
> one of the creators of the dataset, asked me to include a note here saying that
> the trained model therefore can’t be used in a commerical product.

Since this model is used in Gaze, no commercial use is possible.
The `GazeCapture` pipeline makes use of the
[CNN iTracker](http://gazecapture.csail.mit.edu) by Krafka et al., which also
only allows research usage.
