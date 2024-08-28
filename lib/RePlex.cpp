// lib/RePlex.cpp

#include "pub/RePlex.h"
#include <cstdio>

void *Load(const char *filepath){

    return dlopen(filepath, RTLD_NOW);  //loads file from disk to memory
}

void *LoadSymbol(void *library, const char *symbol){

    return dlsym(library, symbol);  //returns address of a given symbol
}

void Reload(void *&library, const char *filepath){

    dlclose(library);  //releases reference to a library 
    library = dlopen(filepath, RTLD_NOW);   //reload other lib
}

void PrintError(){
    printf("ERROR: %s\n", dlerror());
}


