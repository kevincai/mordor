// Copyright (c) 2009 - Decho Corp.

#include "test.h"

#include <cassert>
#include <iostream>

#include "mordor/common/version.h"

#ifdef WINDOWS
#include <windows.h>
#endif

static AllTests *g_allTests;

static struct CleanupAllTests {
    ~CleanupAllTests()
    {
        if (g_allTests)
            delete g_allTests;
    }
} g_cleanupAllTests;

void
registerTest(const std::string &suite, const std::string &testName,
             TestDg test)
{
    if (!g_allTests)
        g_allTests = new AllTests;
    (*g_allTests)[suite].second[testName] = test;
}

void
registerSuiteInvariant(const std::string &suite, TestDg invariant)
{
    if (!g_allTests)
        g_allTests = new AllTests;
    assert((*g_allTests)[suite].first == NULL);
    (*g_allTests)[suite].first = invariant;
}

void
runTests()
{
    TestInstance instance;
    if (g_allTests) {
        for (AllTests::const_iterator it(g_allTests->begin());
            it != g_allTests->end();
            ++it) {
            instance.m_suite = it->first;
            for (TestSuite::second_type::const_iterator
                    it2(it->second.second.begin());
                it2 != it->second.second.end();
                ++it2) {
                if (it->second.first) {
                    instance.m_test = "<invariant>";
                    instance.run(it->second.first);
                }
                instance.m_test = it2->first;
                instance.run(it2->second);
            }
            if (it->second.first) {
                instance.m_test = "<invariant>";
                instance.run(it->second.first);
            }
        }
    }
}

class TestAssertion : public std::exception
{
public:
    TestAssertion(const char *file, int line, const std::string &msg)
        : m_file(file), m_line(line), m_msg(msg)
    {}
   ~TestAssertion() throw() {}

    const char *file() const { return m_file; }
    int line() const { return m_line; }
    const char *what() const throw() { return m_msg.c_str(); }

private:
    const char *m_file;
    int m_line;
    std::string m_msg;
};

void
TestInstance::assertion(const char *file, int line, const std::string &expr)
{
    throw TestAssertion(file, line, expr);
}

void
TestInstance::run(TestDg test)
{
    std::cout << "Running " << m_suite << "::" << m_test << ": ";
    bool protect = true;
#ifdef WINDOWS
    protect = !IsDebuggerPresent();
#endif
    if (protect) {
        try {
            test();
            std::cout << "OK" << std::endl;
        } catch (const TestAssertion &assertion) {
            std::cerr << "Assertion failed (" << assertion.file() << ":"
                << assertion.line() << "):" << std::endl << assertion.what()
                << std::endl;
        } catch (std::exception &ex) {
            std::cerr << "Unexpected exception " << typeid(ex).name() << ":" << std::endl
                << "" << ex.what() << std::endl;
        } catch (...) {
            std::cerr << "Unexpected unknown exception" << std::endl;
        }
    } else {
        test();
        std::cout << "OK" << std::endl;
    }
}
