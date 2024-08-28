#pragma once //tells the compiler to only include a header file once when compiling a source code

#include<dlfcn.h>
#include<array>
#include<unordered_map>
#include<string>

template <typename E, size_t NumSymbols>        //compiler substitutes any desired data type on template class, so teh class becoem suitable for thta type


class RePlexModule{

public:

    static void LoadLibrary() {GetInstance().Load();}
    static void ReloadLibrary() {GetInstance().Reload();}

protected:

//get instance is static as we want only 1 instance of the derived class lib to exist
    static E &GetInstance(){
        static E instance;      //substitutes E with a datatype, passed later
        return instance;
    }

    //return path to lib on disk
    virtual const char *GetPath() const = 0;        //ovveride while implementing

    //returns ref to array used to store symbols as C-strings
    virtual std::array<const char*, NumSymbols> &GetSymbolNames() const = 0;            //ovveride while implementing

    //Ret takes a func name and args, return type is Ret. Args... represents variable number and types of args set that can be passed 
    //eg: Execute<char, int, float>("stuff", 7, 2.4f); Compiler turns this into something like: char Execute(const char* name, int arg1, float arg2)
    template <typename Ret, typename... Args>
    Ret Execute(const char* name, Args... args)
    {
        // Lookup the function address
        auto symbol = m_symbols.find(name);
        if (symbol != m_symbols.end())
        {
        // Cast the address to the appropriate function type and call it,
        // forwarding all arguments
        return reinterpret_cast<Ret(*)(Args...)>(symbol->second)(args...);
        }
        else
        {
        throw std::runtime_error(std::string("Function not found: ") + name);
        }
    }

    // look up the symbol and cast it as a pointer to specified template type
    template <typename T>
    T* GetVar(const char* name)
    {
        auto symbol = m_symbols.find(name);
        if (symbol != m_symbols.end())
        {
        return static_cast<T*>(symbol->second);
        }
        else
        {
        // We didn't find the variable. Return an empty pointer
        return nullptr;
        }
    }

private:

    void Load(){
        m_libHandle = dlopen(GetPath(), RTLD_NOW);
        LoadSymbols();
    }

    void Reload(){
        dlclose(m_libHandle);
        m_symbols.clear();
        Load();
    }

    void LoadSymbols(){
        //GET = symbol : address mapping
        for (const char* symbol : GetSymbolNames())
        {
            m_symbols[symbol] = dlsym(m_libHandle, symbol);
        }
    }

    void *m_libHandle;          //ptr to lib handle
    //symbol-address mapping
    std::unordered_map<std::string, void*> m_symbols;

};

/*
> GetSymbols and populates m_symbols. 
> Load loads all symbols but also calls LoadSymbols. 
> Reload loads all symbols but clears the contents of m_symbols first to ensure there aren’t any invalid symbols lingering around. 
> Load and Reload are called by the static functions LoadLibrary and ReloadLibrary defined above respectively.
> Refinement: instead of hashmap, store address in an array. symbol-address index corresponds in the arrays
*/