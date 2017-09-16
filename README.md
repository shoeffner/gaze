# gaze

## Building

Create a `build` directory and call `cmake`, followed by `make` to build the
gaze library:

```bash mkdir build cd build cmake ..  make ```

You can use several arguments with cmake to modify what is built and how:

- `-DOpenCV_VERSION=<version>`: Defaults to `3.3.0`, but can be changed to try
  to build with other (possibly preinstalled) versions.

Because I keep forgetting them, here some other useful flags:

- `-DCMAKE_VERBOSE_MAKEFILE=ON`: Prints the makefile commands, useful to debug
  them.


### Demos

To build the demo programs, you need to call cmake with `-DBUILD_EXAMPLES=ON`
and an additional `make install` step:

```bash mkdir build cd build cmake -DBUILD_EXAMPLES=ON ..  make make install
```

This also provides other flags:

- `-DBoost_VERSION=<version>`: Defaults to `1.65.0`, but can be overwritten to
  try to build with other (possibly preinstalled) versions.
