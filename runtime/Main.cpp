// runtime/Main.cpp

#include <RePlex.h>
#include <iostream>
#include </Users/suyog_k/Desktop/code-reloader/test/pub/Test.h>

int main(){
    TestModule::LoadLibrary();
    TestModule::Foo();
    std::cout << "bar == "<< TestModule::GetBar() << std::endl;

    std::cout << "Make some changes, recompile and press 'enter'."<<std::flush;
    while(std::cin.get() != '\n') {}

    TestModule::ReloadLibrary();
    TestModule::Foo();
    std::cout << "bar == "<< TestModule::GetBar() << std::endl;
    return 0;
}