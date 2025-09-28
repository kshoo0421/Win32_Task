#include "pch.h"
#include "CppUnitTest.h"
#include "../StringClass/String.h"  // String.h 경로 맞게 수정

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StringUnitTest
{
    TEST_CLASS(StringTests)
    {
    public:

        TEST_METHOD(ConstructorTest)
        {
            String s("Hello");
            Assert::AreEqual((size_t)5, s.size());
            Assert::AreEqual("Hello", s.c_str());
        }

        TEST_METHOD(CopyConstructorTest)
        {
            String a("World");
            String b(a);
            Assert::AreEqual(a.c_str(), b.c_str());
            Assert::AreEqual(a.size(), b.size());
        }

        TEST_METHOD(MoveConstructorTest)
        {
            String a("Temp");
            String b(std::move(a));
            Assert::AreEqual("Temp", b.c_str());
            Assert::AreEqual((size_t)4, b.size());
            Assert::AreEqual("", a.c_str()); // a는 비워져야 함
        }

        TEST_METHOD(AssignmentOperatorTest)
        {
            String a("Hello");
            String b("World");
            b = a;
            Assert::AreEqual("Hello", b.c_str());
        }

        TEST_METHOD(MoveAssignmentTest)
        {
            String a("MoveMe");
            String b;
            b = std::move(a);
            Assert::AreEqual("MoveMe", b.c_str());
            Assert::AreEqual("", a.c_str());
        }

        TEST_METHOD(ConcatOperatorTest)
        {
            String a("Hello");
            String b("World");
            String c = a + b;
            Assert::AreEqual("HelloWorld", c.c_str());
            Assert::AreEqual((size_t)10, c.size());
        }

        TEST_METHOD(AppendTest)
        {
            String s("Data");
            s.append("Base");
            Assert::IsTrue(std::strcmp("DataBase", s.c_str()) == 0);
            Assert::AreEqual((size_t)8, s.size());
        }

        TEST_METHOD(ReserveAndCapacityTest)
        {
            String s("Hi");
            size_t oldCap = s.get_capacity();
            s.reserve(oldCap * 2);
            Assert::IsTrue(s.get_capacity() >= oldCap * 2);
            Assert::AreEqual("Hi", s.c_str()); // 데이터는 그대로 유지돼야 함
        }

        TEST_METHOD(IndexOperatorTest)
        {
            String s("Test");
            Assert::AreEqual<char>('T', s[0]);
            Assert::AreEqual<char>('t', s[3]);
            Assert::AreEqual<char>('s', s[2]);
            s[0] = 'B';
            Assert::IsTrue(std::strcmp("Best", s.c_str()) == 0);
        }

        TEST_METHOD(EqualityOperatorsTest)
        {
            String a("Alpha");
            String b("Alpha");
            String c("Beta");

            Assert::IsTrue(a == b);
            Assert::IsTrue(a != c);
        }
    };
}
