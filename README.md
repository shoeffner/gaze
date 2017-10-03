# gaze

## Building

Create a `build` directory and call `cmake`, followed by `make` to build the
gaze library:

```bash
mkdir
build
cd build
cmake ..
make
```

You can use several arguments with cmake to modify what is built and how:

- `-DOpenCV_VERSION=<version>`: Defaults to `3.3.0`, but can be changed to try
  to build with other (possibly preinstalled) versions.

Because I keep forgetting them, here some other useful flags:

- `-DCMAKE_VERBOSE_MAKEFILE=ON`: Prints the makefile commands, useful to debug
  them.


### Demos

To build the demo programs, you need to call cmake with `-DBUILD_EXAMPLES=ON`
and an additional `make install` step:

```bash
mkdir build
cd build
cmake -DBUILD_EXAMPLES=ON ..
make
make install
```

This also provides other flags:

- `-DBoost_VERSION=<version>`: Defaults to `1.65.0`, but can be overwritten to
  try to build with other (possibly preinstalled) versions.

The demo programs are available inside the build directory after the build
process.


#### `gaze_debug_viewer`

The `gaze_debug_viewer` simply plays back a video file or opens a webcam stream.

```txt
Command line arguments:
  --help                  Shows this help message
  --input_source arg (=0) The input source. An integer for webcam access
                          (usually 0), or a string as a path to a video file.
```

Example: `./gaze_debug_viewer` opens the webcam stream. `./gaze_debug_viewer 0`
and `./gaze_debug_viewer --input_source 0` are equivalent. You can specify a
video file as well: `./gaze_debug_viewer my_video.avi`.


#### `where_people_look`

This is a reimplementation of Judd et al. (2009)'s data acquisition experiment \cite judd2009.

It needs to be *installed* using `make install` to put the images into the
correct location. You can run it by simply typing `./where_people_look` from
the build directory.
