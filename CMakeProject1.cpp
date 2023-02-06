// CMakeProject1.cpp : Defines the entry point for the application.

#ifdef WIN32
#include <windows.h>
#endif // WIN32
#include <iostream>


template <class Ch, class Tr, class Last>
void print(std::basic_ostream<Ch, Tr>&os, const Last& last) 
{
    os << last << "\n";
}

template <class Ch, class Tr, class First, class... Other>
void print(std::basic_ostream<Ch, Tr>& os,
    const First& first, const Other&... other)
{
    os << first << ", ";
    print(os, other...);
}

#if 0
#define PRINT1(v)
#define PRINTN(...)
#else
#define PRINT1(v) std::cerr << #v << "=" << v << "\n";
#define PRINTN(...) print(std::cerr, "[" #__VA_ARGS__ "]", __VA_ARGS__);
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

#if 1

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

bool ope(int i, int j) 
{
    return i == j;
}

std::vector<int> ftd = { 1,2,3,7,9 };

bool iod1(int n)
{
    return (std::find_if(ftd.begin(), ftd.end(), 
        std::bind(ope, std::placeholders::_1, n)) != ftd.end());
}

bool iod2(int n)
{
    return (std::find_if(ftd.begin(), ftd.end(), 
        [n](int i) { return i == n; }) != ftd.end());
}


void f()
{
    std::cout << iod1(0) << iod1(1) << iod1(4) << iod1(7) << "\n";
    std::cout << iod2(0) << iod2(1) << iod2(4) << iod2(7) << "\n";
}
#endif // 

#if 0
void f()
{
#define A 1
    const char* b = "QWERT";
    int i = 33;
    double pi = 3.14;
    print(std::cerr,  i, pi);
    PRINT1(A);
    PRINTN(A, b, i, pi);
}
#endif // debug PRINT1/PRINTN
#if 0
// https://habr.com/ru/company/otus/blog/546158/
#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

// the variant to visit
using var_t = std::variant<int, long, double, std::string>;

// helper constant for the visitor #3
template<class> inline constexpr bool always_false_v = false;

// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

struct gg {
    template <class T>
    void operator()(const T& arg) { std::cout  << "<>" << c << arg; }
    void operator()(const int& arg) { std::cout << "int" << c << arg; }
    char c;
};

void f()
{
    std::vector<var_t> vec = { 10, 15l, 1.5, "hello" };
    for (auto& v : vec) {

        // 1. void visitor, only called for side-effects (here, for I/O)
        auto ff = [](auto&& arg) {std::cout << arg; };
        std::visit(gg{':'}, v);

        // 2. value-returning visitor, demonstrates the idiom of returning another variant
        var_t w = std::visit([](auto&& arg) -> var_t {return arg + arg; }, v);

        // 3. type-matching visitor: a lambda that handles each type differently
        std::cout << ". After doubling, variant holds ";
        std::visit([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>)
                std::cout << "int with value " << arg << '\n';
            else if constexpr (std::is_same_v<T, long>)
                std::cout << "long with value " << arg << '\n';
            else if constexpr (std::is_same_v<T, double>)
                std::cout << "double with value " << arg << '\n';
            else if constexpr (std::is_same_v<T, std::string>)
                std::cout << "std::string with value " << std::quoted(arg) << '\n';
            else
                static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }, w);
    }

    for (auto& v : vec) {
        // 4. another type-matching visitor: a class with 3 overloaded operator()'s
        // Note: The `(auto arg)` template operator() will bind to `int` and `long`
        //       in this case, but in its absence the `(double arg)` operator()
        //       *will also* bind to `int` and `long` because both are implicitly
        //       convertible to double. When using this form, care has to be taken
        //       that implicit conversions are handled correctly.
        std::visit(overloaded{
            [](auto arg) { std::cout << arg << ' '; },
            [](double arg) { std::cout << std::fixed << arg << ' '; },
            [](const std::string& arg) { std::cout << std::quoted(arg) << ' '; }
            }, v);
    }
}
#endif //  std::variant + std::visit
#if 0

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
#endif // converting unicode using <codecvt>
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
        std::cout << "\n>>>>>>>>>>>>>>\n";
        return 0;
}
