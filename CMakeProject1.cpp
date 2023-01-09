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

#if 0
void f()
{
}
#endif // 

#if 0
void f()
{
}
#endif // 

#if 1

#include <fstream>
#include <string>
#include <locale>
#include <iomanip>
#include <codecvt>
#include <cstdint>

// utility wrapper to adapt locale-bound facets for wstring/wbuffer convert
template<class Facet>
struct deletable_facet : Facet
{
    template<class ...Args>
    deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}
    ~deletable_facet() {}
};
void f()
{
    // UTF-8 narrow multibyte encoding
    std::string data = reinterpret_cast<const char*>(+u8"z\u00df\u6c34\U0001f34c");
    // or reinterpret_cast<const char*>(+u8"zß水🍌")
    // or "\x7a\xc3\x9f\xe6\xb0\xb4\xf0\x9f\x8d\x8c"

    std::ofstream("text.txt") << data;

    // using system-supplied locale's codecvt facet
    std::wifstream fin("text.txt");
    // reading from wifstream will use codecvt<wchar_t, char, mbstate_t>
    // this locale's codecvt converts UTF-8 to UCS4 (on systems such as Linux)
    fin.imbue(std::locale("en_US.UTF-8"));
    std::cout << "The UTF-8 file contains the following UCS4 code units:   ";
    for (wchar_t c; fin >> c; )
        std::cout << "U+" << std::hex << std::setw(4) << std::setfill('0')
        << static_cast<uint32_t>(c) << ' ';

    // using standard (locale-independent) codecvt facet
    std::wstring_convert<
        deletable_facet<std::codecvt<char16_t, char, std::mbstate_t>>, char16_t> conv16;
    std::u16string str16 = conv16.from_bytes(data);

    std::cout << "\nThe UTF-8 file contains the following UTF-16 code units: ";
    for (char16_t c : str16)
        std::cout << "U+" << std::hex << std::setw(4) << std::setfill('0')
        << static_cast<uint16_t>(c) << ' ';
}
#endif // 

#if 0
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

void f()
{
    // UTF-8 data. The character U+1d10b, musical sign segno, does not fit in UCS2
    std::string utf8 = u8"z\u6c34\U0001d10b";

    // the UTF-8 / UTF-16 standard conversion facet
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16conv;
    std::u16string utf16 = utf16conv.from_bytes(utf8);
    std::cout << "UTF16 conversion produced " << utf16.size() << " code units:\n";
    for (char16_t c : utf16)
        std::cout << std::hex << std::showbase << c << '\n';

    // the UTF-8 / UCS2 standard conversion facet
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> ucs2conv;
    try {
        std::u16string ucs2 = ucs2conv.from_bytes(utf8);
    }
    catch (const std::range_error& e) {
        std::u16string ucs2 = ucs2conv.from_bytes(utf8.substr(0, ucs2conv.converted()));
        std::cout << "UCS2 failed after producing " << std::dec << ucs2.size() << " characters:\n";
        for (char16_t c : ucs2)
            std::cout << std::hex << std::showbase << c << '\n';
    }
}
#endif // standard unicode C++ conversion sample

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
