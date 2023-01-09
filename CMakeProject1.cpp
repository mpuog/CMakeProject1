// CMakeProject1.cpp : Defines the entry point for the application.

#ifdef WIN32
#include <windows.h>
#endif // WIN32
#include <iostream>
#if 0
#define PRINT1(v)
#else
#define PRINT1(v) std::cout << #v << "="<< v << "\n";
#endif // 0



#if 1
# define TRACE_KLASS(T) T

class A  // тестовый класс для примеров 
{
public:
    int a;
    operator int() const { TRACE_KLASS(std::cout << " int(" << a << ") ";) return a; }
    A(int i = 0) : a(i) { TRACE_KLASS(std::cout << " +A(" << a << ") ";) }
    A(const A& a_) : a(a_.a) { TRACE_KLASS(std::cout << " &A(" << a << ") ";) }
    A& operator = (const A& a_) { a = a_.a; TRACE_KLASS(std::cout << " =A(" << a << ") ";) return *this; }
    A(A&& a_) : a(std::move(a_.a)) {
        TRACE_KLASS(std::cout << " &&A(" << a << ") ";) a_.a = 99999999;
    }
    A& operator=(A&& _a) {
        a = std::move(_a.a);
        TRACE_KLASS(std::cout << " &&=(" << a << ") ";)
            _a.a = 88888888;
        return *this;
    }
    ~A() { TRACE_KLASS(std::cout << " -A(" << a << ") ";) }
    bool operator < (const A& a_) const { TRACE_KLASS(std::cout << " <A(" << a << ") ";) return a < a_.a; }
    void fconst(int i, int j) const { TRACE_KLASS(std::cout << " fconst(" << a << "|" << i << "|" << j << ") ";) }
    void fvar(int i, int j) { TRACE_KLASS(std::cout << " fvar(" << a << "|" << i << "|" << j << ") ";) }
};

inline void swap(A& a1, A& a2)
{
    TRACE_KLASS(std::cout << " (" << a1.a << "<==>" << a2.a << ") ";)
        std::swap(a1.a, a2.a);
}
#endif  // тестовый класс A


#if 0
void f()
{
}
#endif // 

#if 1

struct B
{
    A a;
    //B & 
};

A fa()
{
    A a(4);
    return a;
}
void f()
{
    auto x = fa();
    //B b;
    //B b1;
    //b1 = std::move(b);
}
#endif // 

#if 0
void f()
{
    std::string s=u8"ЫВА";
    std::cin >> s;
    std::cout << u8"Hello CMake. ФЫВА\n" << s << ":\n";
}
#endif // after SetConsole... output utf8 works, input DOESN'T work in Windows

int main()
{
#ifdef WIN32
    // output utf8 работает
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif // WIN32

    std::cout << __DATE__ " " __TIME__ "  "
#ifdef _MSC_VER
        << "MS:" << _MSC_VER
#else
        << "GCC:" << __GNUC__
#endif // _MSC_VER

#ifdef WIN32
        << " WINDOWS\n";
#else
        << " LINUX\n";
#endif // _MSC_VER

        try
        {
            f();
            std::cout << "\nafter f(); ";
        }
        catch (const std::exception& err)
        {
            std::cout << "STD_EXCEPTION:" << err.what() << "\n";
        }
        catch (...)
        {
            std::cout << "UNKNOWN_EXCEPTION\n";
        }
        std::cout << ">>>>>>>>>>>>>>\n";
        return 0;
}
