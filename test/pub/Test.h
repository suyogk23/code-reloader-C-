// test.h

#pragma once

#include </Users/suyog_k/Desktop/code-reloader/lib/pub/RePlex.h>

extern "C"
{
  void foo();
  extern int bar;
}

std::array<const char*, 2> g_exports = {"foo", "bar"};

class TestModule : public RePlexModule <TestModule, g_exports.size()>
{
public:
    static void Foo(){
        GetInstance().Execute<void>("foo");
    }

    static int GetBar(){
        //resolves to symbol dtype "int". 
        return *GetInstance().GetVar<decltype(bar)>("bar");
    }

protected:

    virtual const char *GetPath() const override{

#ifdef DEBUG
    return "bin/Debug/libRePlexTest.dylib";
#else
    return "bin/Release/libRePlexTest.dylib";
#endif

    }

    virtual std::array<const char*, g_exports.size()> &GetSymbolNames() const override{
        return g_exports;
    }

};