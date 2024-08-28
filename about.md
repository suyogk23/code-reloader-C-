# code reloader:
A code reloader, also known as hot reload, hot deployment, or hot swapping, is a development technique that allows developers to make changes to their codebase while the application is running. This means that developers can apply changes to a running program without the need to stop or restart the application, or go through the build package or deploy steps.

# componenets:
- shared lib: act as a live code-reloader
- runtime

# Build process:
RePlex
|-- premake5.lua
|-- lib
|   |-- RePlex.cpp
|   +-- pub
|       +-- RePlex.h
+-- runtime
    +-- Main.cpp

- pub will contain public headers

# Symbols
- Symbols: used by linker to determine addr space of a function, var, etc... Symbols are used by linker @ compile time(locates memory address of instruction). 
UNIX APIs fro symbol associations:
- dlopen: Given a file name, reads the library from disk into memory
- dlsym: Given a symbol, returns the address of that symbol
- dlerror: Returns an error message describing the last thing that went wrong
- dlclose: Releases a reference to the specified library. If the reference count drops to zero, the library is removed from the address space.

- Quick test:
+------+
|RePlex| Library for performing loading and unloading
+------+
 |       \  Is a dependency of
 |        \
 |         *---*
 |              \     +-------------+
 |               *--> |RePlexRuntime|
 |                    +-------------+
 | Loads               /
 |                    / Uses symbols in RePlexTest
  \                  /
   \     +----------+
    *--> |RePlexTest| Library that gets reloaded/ hot-loaded while running
         +----------+