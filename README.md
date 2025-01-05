## A repo to demonstrate `libarchive` usage

### Dependencies
`libarchive` requires `zlib`. `zlib` is installed on most `UNIX` systems by default. If not, install it using your system-specific package manager such as `apt` or `brew`. 

### Demo
1. Clone the repo
    * `git clone https://github.com/nav-mohan/example_libarchive`
2. Initialize and update the submodule `libarchive` 
    * `git submodule init`
    * `git submodule update`
3. Compile
    * `mkdir build`
    * `cd build`
    * `cmake ..`
    * `make`

> [!NOTE]  
> Many flags listed in `libarchive_flags.cmake` have been toggled `OFF` (such as `TESTS`, `OPENSSL`, `UNZIP`) because they were not required for my project. I've also disabled other dependancies such as `zlib`, and `tar` to significantly reduce compile-time.

> [!NOTE]  
> In this example, I've chosen to build the `static` library and linked against the `libarchive_static`. To build the shared-libarary version, toggle the flag `BUILD_SHARED_LIBS` and link against `libarchive`.

> [!NOTE]  
> Compilation of `libarchive/libarchive/archive_write_set_format_zip.c` throws an `unused variable` error for `char finishing`. Commenting it out fixes it.

