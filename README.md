## Bachelor-diploma-at-University
 This app was my bachelor diploma at NURE University in 2018.

### Build and run
You need to install Qt framework to be able to build and run the app.
The application now runs on Qt6.
Specify a path to your Qt binaries using `-DCUSTOM_CMAKE_PREFIX_PATH` variable if you use `CMake`.
If you use `Make` just add the path to your Qt binaries to variable `QT_BINARIES_PATH` in `Makefile` 
and then you will be able to use a simple `Make`-based interface such as:
```shell
    make clean
    make build
    make unit-tests
    make run
```
Build dirs names are stored in `Scripts/env.sh` script as the variables `DEBUG_BUILD_DIR"`
`RELEASE_BUILD_DIR"` with default values, so if you need you can change it.



