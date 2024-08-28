// runtime/Main.cpp

#include <gtest/gtest.h>
#include <cstdlib>
#include <fstream>
#include <RePlex.h>
#include <Test.h>

const char *g_Test_v1 = 
    "#include \"/Users/suyog_k/Desktop/code-reloader/test/pub/Test.h\"\n"
    "int bar = 300;\n"
    "int foo(int x)\n"
    "{\n"
    "   return x%2;\n"
    "}";

const char *g_Test_v2 =
  "#include \"/Users/suyog_k/Desktop/code-reloader/test/pub/Test.h\"\n"
  "int bar = -8950;\n"
  "int foo(int x)\n"
  "{\n"
  "  return x * 5;\n"
  "}";

class RePlexTest : public ::testing::Test{
public:
    //auto call @ start of each test case
    virtual void SetUp(){
        WriteFile("test/Test.cpp", g_Test_v1);
        Compile(1);
        TestModule::LoadLibrary();
    }

    //invoke manualy inmiddle of each test case
    void ChangeAndReload(){
        WriteFile("test/Test.cpp", g_Test_v2);
        Compile(2);
        TestModule::ReloadLibrary();
    }

    //called automatically at end of each test case
    virtual void TearDown(){
        TestModule::ReloadLibrary();
        WriteFile("test/Test.cpp", g_Test_v1);
        Compile(1);
    }

private:
    void WriteFile(const char *path, const char *text){
        //open O/P fstream deleting existinf file contents
        std::ofstream out(path, std::ios_base::trunc | std::ios_base::out);
        out<<text;
    }
    void Compile(int version){
        if(version == m_version){
            return;
        }

        m_version = version;
        EXPECT_EQ(std::system("make"), 0);
        //sleep: until make is fully flused, wait
        sleep(1);
    }
    
    //keep track of currently loaded lib version
    int m_version = 1;
};

TEST_F (RePlexTest, VariableReload){
    EXPECT_EQ(TestModule::GetBar(), 300);
    ChangeAndReload();
    EXPECT_EQ(TestModule::GetBar(), -8950);
}

TEST_F (RePlexTest, FunctionReload){
    EXPECT_EQ(TestModule::Foo(20), 0);
    ChangeAndReload();
    EXPECT_EQ(TestModule::Foo(5), 25);
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}